#include "Buffer.hpp"

#include <cstring>
#include <iostream>

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
    glUnmapNamedBuffer(m_name);
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (m_managed) [[likely]] return;
    glDeleteBuffers(1, &m_name);
}

//------------------------------------------------------------------------

void Buffer::bind() const noexcept
{
    glBindBuffer(BufferUsage2GLenum[m_usage], m_name);
}

//------------------------------------------------------------------------

void Buffer::bindBase(GLuint bindingIndex) const noexcept
{
    if (m_usage != BufferUsage::UNIFORM && m_usage != BufferUsage::STORAGE) [[unlikely]]
    {
        std::cerr << "Bind base of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferBase(BufferUsage2GLenum[m_usage], bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr byteSize) const noexcept
{
    if (m_usage != BufferUsage::UNIFORM && m_usage != BufferUsage::STORAGE) [[unlikely]]
    {
        std::cerr << "Bind range of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferRange(BufferUsage2GLenum[m_usage], bindingIndex, m_name, offsetBytes, byteSize);
}

//------------------------------------------------------------------------

void Buffer::freeResources() const noexcept
{
    glDeleteBuffers(1, &m_name);
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
