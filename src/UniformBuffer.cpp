#include "UniformBuffer.hpp"

//------------------------------------------------------------------------

UniformBuffer::UniformBuffer(const std::string& blockName, uint32_t binding, std::ptrdiff_t blockSize,
    uint32_t extent, uint32_t usage)
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

void UniformBuffer::update(std::ptrdiff_t offset, const void* data, std::size_t size, std::ptrdiff_t blockOffset) const noexcept
{
    glBindBuffer(GL_UNIFORM_BUFFER, m_handle);
    glBufferSubData(GL_UNIFORM_BUFFER, blockOffset * m_blockSize + offset, size, data);
}

//------------------------------------------------------------------------
