#include "Buffer.hpp"

#include <cstring>
#include <print>

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

bool Buffer::baseOrRangeBindable() const noexcept
{
    return m_usage == BufferUsage::UNIFORM
        or m_usage == BufferUsage::STORAGE
        or m_usage == BufferUsage::ATOMIC_COUNTER
        or m_usage == BufferUsage::PARAMETER;
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
    if (not baseOrRangeBindable()) [[unlikely]] {
        std::println("Bind base of non-UBO or non-SSBO");
        return;
    }
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
    if (not baseOrRangeBindable()) [[unlikely]] {
        std::println("Bind range of non-UBO or non-SSBO");
        return;
    }
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
