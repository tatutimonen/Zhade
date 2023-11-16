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

    glVertexArrayVertexBuffer(m_vao, 0, m_scene->vertexBuffer()->name(), 0, sizeof(Vertex));
    glVertexArrayElementBuffer(m_vao, m_scene->indexBuffer()->name());

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
    m_drawID2ModelBuffer = m_mngr->createBuffer(desc.drawID2ModelBufferDesc);
    m_textureBuffer = m_mngr->createBuffer(desc.textureBufferDesc);

    glBindVertexArray(m_vao);
    commandBuffer()->bind();
    transformBuffer()->bindBase(MODEL_BINDING);
    drawID2ModelBuffer()->bindBase(DRAW_ID_2_MODEL_IDX_BINDING);
    textureBuffer()->bindBase(TEXTURE_BINDING);
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    m_mngr->destroy(m_commandBuffer);
    m_mngr->destroy(m_transformBuffer);
    m_mngr->destroy(m_drawID2ModelBuffer);
    m_mngr->destroy(m_textureBuffer);
}

//------------------------------------------------------------------------

void Renderer::render() const noexcept
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, drawCount(), 0);
}

//------------------------------------------------------------------------

void Renderer::processSceneGraph() const noexcept
{
    if (m_scene->models().empty()) [[unlikely]] return;
    stdr::sort(
        m_scene->m_models,
        std::less{},
        [this](const auto& handle) { return m_mngr->get(handle)->id(); }
    );
    populateBuffers();
}

//------------------------------------------------------------------------

void Renderer::populateBuffers() const noexcept
{
    invalidateBuffers();

    for (const auto& modelHandle : m_scene->models())
    {
        const Model* model = m_mngr->get(modelHandle);
        for (const auto& meshHandle : model->meshes())
        {
            const Mesh* mesh = m_mngr->get(meshHandle);
            DrawElementsIndirectCommand cmd{
                .count = mesh->numIndices(),
                .instanceCount = 1,
                .firstIndex = mesh->firstIndex(),
                .baseVertex = mesh->baseVertex(),
                .baseInstance = 0
            };
            pushMeshDataToBuffers(mesh);
            commandBuffer()->pushData(&cmd);
        }
    }
}

//------------------------------------------------------------------------

void Renderer::invalidateBuffers() const noexcept
{
    commandBuffer()->invalidate();
    transformBuffer()->invalidate();
    drawID2ModelBuffer()->invalidate();
    textureBuffer()->invalidate();
}

//------------------------------------------------------------------------

void Renderer::pushMeshDataToBuffers(const Mesh* mesh) const noexcept
{
    static uint32_t prevModelId = std::numeric_limits<uint32_t>::max();
    const Model* model = m_mngr->get(mesh->model());
    const uint32_t modelId = model->id();

    if (modelId != prevModelId)
    {
        transformBuffer()->pushData(&model->transformation());
    }

    drawID2ModelBuffer()->pushData(&modelId);
    const GLuint64 diffuseHandle = m_mngr->get(mesh->diffuse())->handle();
    textureBuffer()->pushData(&diffuseHandle);

    prevModelId = modelId;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
