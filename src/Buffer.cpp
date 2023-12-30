#include "Buffer.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Buffer::Buffer(BufferDescriptor desc)
    : m_usage{desc.usage},
      m_wholeByteSize{desc.byteSize},
      m_managed{desc.managed}
{
    glCreateBuffers(1, &m_name);
    glNamedBufferStorage(m_name, m_wholeByteSize, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
    m_ptr = std::bit_cast<uint8_t*>(glMapNamedBufferRange(m_name, 0, m_wholeByteSize, s_access));
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (m_managed) [[likely]] return;
    freeResources();
}

//------------------------------------------------------------------------

void Buffer::freeResources() const noexcept
{
    glUnmapNamedBuffer(m_name);
    glDeleteBuffers(1, &m_name);
}

//------------------------------------------------------------------------

void Buffer::bind() const noexcept
{
    glBindBuffer(BufferUsage2GLenum[m_usage], m_name);
}

//------------------------------------------------------------------------

void Buffer::bindAs(BufferUsage::Type usage) const noexcept
{
    glBindBuffer(BufferUsage2GLenum[usage], m_name);
}

//------------------------------------------------------------------------

void Buffer::bindBase(GLuint bindingIndex) const noexcept
{
    glBindBufferBase(BufferUsage2GLenum[m_usage], bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindBaseAs(GLuint bindingIndex, BufferUsage::Type usage) const noexcept
{
    glBindBufferBase(BufferUsage2GLenum[usage], bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr byteOffset, GLsizeiptr byteSize) const noexcept
{
    glBindBufferRange(BufferUsage2GLenum[m_usage], bindingIndex, m_name, byteOffset, byteSize);
}

//------------------------------------------------------------------------

void Buffer::invalidate(GLintptr offset, GLsizeiptr length) const noexcept
{
    glInvalidateBufferSubData(m_name, offset, length == 0 ? m_writeOffset : length);
    m_writeOffset = 0;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
