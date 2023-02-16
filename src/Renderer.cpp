#include "Renderer.hpp"

#include <ranges>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Renderer::Renderer(const RendererSpec& spec, const ResourceManager* mngr)
    : m_mngr{mngr},
      m_programs{spec.programs}
{
    glCreateVertexArrays(1, &m_vao);

    glEnableVertexArrayAttrib(m_vao, 0);
    glEnableVertexArrayAttrib(m_vao, 1);
    glEnableVertexArrayAttrib(m_vao, 2);

    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos));
    glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm));
    glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, tex));

    for (GLuint idx : std::views::iota(0u, spec.vertexBuffers.size()))
    {
        const GLuint vboHandle = m_mngr(spec.vertexBuffers.at(idx))->getGLHandle();
        glVertexArrayVertexBuffer(m_vao, idx, vboHandle, 0, sizeof(Vertex));
        glVertexArrayAttribBinding(m_vao, 0, idx);
        glVertexArrayAttribBinding(m_vao, 1, idx);
        glVertexArrayAttribBinding(m_vao, 2, idx);
    }

    glVertexArrayElementBuffer(m_vao, m_mngr(spec.indexBuffer)->getGLHandle());
}

//------------------------------------------------------------------------

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
