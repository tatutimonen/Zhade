#include "Renderer.hpp"

#include "common.hpp"

#include <array>
#include <cstring>
#include <memory_resource>
#include <numeric>
#include <ranges>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(RendererDescriptor desc)
    : m_mngr{desc.mngr},
      m_scene{desc.scene}
{
    glCreateVertexArrays(1, &m_vao);

    glVertexArrayVertexBuffer(m_vao, 0, m_scene->vertexBuffer()->getName(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_scene->indexBuffer()->getName());

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));

    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    m_commandBuffer = m_mngr->createBuffer(desc.commandBufferDesc);
    m_transformBuffer = m_mngr->createBuffer(desc.transformBufferDesc);
    m_textureBuffer = m_mngr->createBuffer(desc.textureBufferDesc);

    glBindVertexArray(m_vao);
    commandBuffer()->bind();
    transformBuffer()->bindBase(MODEL_BINDING);
    textureBuffer()->bindBase(TEXTURE_BINDING);
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    m_mngr->destroy(m_commandBuffer);
    m_mngr->destroy(m_transformBuffer);
    m_mngr->destroy(m_textureBuffer);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    processSceneGraph();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, drawCount(), 0);
}

//------------------------------------------------------------------------

void Renderer::processSceneGraph() const noexcept
{
    if (m_scene->getModels().empty()) [[unlikely]] return;

    // Collect the scene meshes to a local container and sort according to ID.
    static std::array<uint8_t, KIB_BYTES> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<Handle<Mesh>> meshes{&rsrc};
    for (const auto& model : m_scene->getModels())
    {
        meshes.append_range(m_mngr->get(model)->meshes());
    }
    stdr::sort(
        meshes,
        std::less{},
        [this](const auto& handle) { return m_mngr->get(handle)->id(); }
    );

    populateBuffers(meshes);
}

//------------------------------------------------------------------------

void Renderer::populateBuffers(std::span<Handle<Mesh>> meshesSorted) const noexcept
{
    invalidateBuffers();

    auto mesh = m_mngr->get(meshesSorted.front());
    DrawElementsIndirectCommand cmd{
        .vertexCount = mesh->numVertices(),
        .instanceCount = 1,
        .firstIndex = mesh->firstIndex(),
        .baseVertex = mesh->baseVertex(),
        .baseInstance = 0
    };

    pushMeshDataToBuffers(mesh);
    if (meshesSorted.size() == 1)
    {
        commandBuffer()->pushData(&cmd);
        return;
    }

    for (const auto& meshHandle : stdr::drop_view(meshesSorted, 1))
    {
        auto prevMesh = mesh;
        mesh = m_mngr->get(meshHandle);

        if (mesh->id() != prevMesh->id())
        {
            commandBuffer()->pushData(&cmd);
            cmd = {
                .vertexCount = mesh->numVertices(),
                .instanceCount = 0,
                .firstIndex = mesh->firstIndex(),
                .baseVertex = mesh->baseVertex(),
                .baseInstance = cmd.baseInstance + cmd.instanceCount
            };
        }

        pushMeshDataToBuffers(mesh);
        ++cmd.instanceCount;
    }
}

//------------------------------------------------------------------------

void Renderer::invalidateBuffers() const noexcept
{
    commandBuffer()->invalidate();
    transformBuffer()->invalidate();
    textureBuffer()->invalidate();
}

//------------------------------------------------------------------------

void Renderer::pushMeshDataToBuffers(const Mesh* mesh) const noexcept
{
    transformBuffer()->pushData(&m_mngr->get(mesh->model())->transformation());
    const auto diffuseGPUHandle = m_mngr->get(mesh->diffuse())->getHandle();
    textureBuffer()->pushData(&diffuseGPUHandle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
