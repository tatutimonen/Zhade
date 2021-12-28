#include "UniformBuffer.hpp"

//------------------------------------------------------------------------

UniformBuffer::UniformBuffer(const std::string& blockName,
    GLuint binding,
    GLsizeiptr blockSize,
    uint32_t extent,
    GLenum usage)
    : m_blockSize{blockSize}
{
    glGenBuffers(1, &m_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferData(GL_UNIFORM_BUFFER, blockSize * extent, nullptr, usage);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

//------------------------------------------------------------------------

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------

void UniformBuffer::update(std::size_t offset, const void* data, std::size_t size, std::size_t blockOffset)
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    GLbyte* buffer = reinterpret_cast<GLbyte*>(glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY));
    std::memcpy(buffer + blockOffset * m_blockSize + offset, data, size);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
}

//------------------------------------------------------------------------
