#include "UniformBuffer.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

UniformBuffer::UniformBuffer(const GLuint binding, const GLsizeiptr blockSize, const GLsizeiptr extent, const GLuint usage)
    : m_binding{binding},
      m_blockSize{blockSize}
{
    glCreateBuffers(1, &m_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferData(GL_UNIFORM_BUFFER, blockSize * extent, nullptr, usage);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle);
}

//------------------------------------------------------------------------

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------

void UniformBuffer::update(const GLsizeiptr offset, const void* data, const size_t size, const GLsizeiptr blockOffset) const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, blockOffset * m_blockSize + offset, size, data);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
