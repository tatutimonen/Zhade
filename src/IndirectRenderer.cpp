#include "IndirectRenderer.hpp"

#include <array>
#include <memory_resource>
#include <numeric>
#include <ranges>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

IndirectRenderer::IndirectRenderer(ResourceManager* mngr, Scene* scene)
    : m_mngr{mngr},
      m_scene{scene}
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

    m_buffers = {
        .commandBuffer = m_mngr->createBuffer({.byteSize = KIB_BYTES*100, .usage = BufferUsage::INDIRECT}),
        .transformBuffer = m_mngr->createBuffer({.byteSize = GIB_BYTES/2, .usage = BufferUsage::STORAGE}),
        .textureBuffer = m_mngr->createBuffer({.byteSize = KIB_BYTES*100, .usage = BufferUsage::STORAGE})
    };
}

//------------------------------------------------------------------------

void IndirectRenderer::render() const noexcept
{
    processSceneGraph();

    for (const auto& renderPass : m_extraPasses)
    {
        m_mngr->get(renderPass.framebuffer)->bind();
        glClear(renderPass.clearMask);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
}

//------------------------------------------------------------------------

void IndirectRenderer::processSceneGraph() const noexcept
{
    static std::array<uint8_t, 2048> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<uint32_t> modelIndices{&rsrc};

    std::iota(modelIndices.begin(), modelIndices.end(), 0u);
    auto idx2model = [this](uint32_t idx) { return m_mngr->get(m_scene->getModels()[idx]); };
    std::ranges::sort(modelIndices, std::less{}, idx2model);

    for (const Model2* model : std::ranges::views::transform(modelIndices, idx2model))
    {

    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
