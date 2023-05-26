#include "IndirectRenderer.hpp"

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

}

//------------------------------------------------------------------------

void IndirectRenderer::setupCommandBuffer() const noexcept
{
    for (const auto& modelHandle : m_scene->getModels())
    {
        const Model2* model = m_mngr->get(modelHandle);
        
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
