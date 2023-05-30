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
      m_scene{scene},
      m_commandBuffer{mngr->createBuffer(GL_DRAW_INDIRECT_BUFFER, 1 << 17)}
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
}

//------------------------------------------------------------------------

void IndirectRenderer::render() const noexcept
{
    prepareForRender();

    static GLuint currentlyBoundFbo{};

    for (const auto& renderPass : m_extraPasses)
    {
        const Framebuffer* framebuffer = m_mngr->get(renderPass.framebuffer);
        const auto fbo = framebuffer->getName();
        if (fbo != currentlyBoundFbo) [[unlikely]]
        {
            currentlyBoundFbo = fbo;
            framebuffer->bind();
        }
        glClear(renderPass.clearMask);
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
}

//------------------------------------------------------------------------

void IndirectRenderer::prepareForRender() const noexcept
{
    static std::array<uint8_t, 2048> buf;
    std::pmr::monotonic_buffer_resource rsrc{buf.data(), buf.size()};
    std::pmr::vector<uint32_t> modelIndices{&rsrc};

    std::iota(modelIndices.begin(), modelIndices.end(), 0u);
    auto idx2model = [this](uint32_t idx){ return m_mngr->get(m_scene->getModels()[idx]); };
    std::ranges::sort(modelIndices, std::less{}, idx2model);

    for (const Model2* model : std::ranges::views::transform(modelIndices, idx2model))
    {

    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
