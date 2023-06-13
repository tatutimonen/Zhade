#include "Buffer.hpp"

#include <cstring>
#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Buffer::Buffer(BufferDescriptor desc)
    : m_usage{desc.usage},
      m_wholeSizeBytes{desc.sizeBytes},
      m_managed{desc.managed}
{
    glCreateBuffers(1, &m_name);
    glNamedBufferStorage(m_name, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
    m_ptr = std::bit_cast<uint8_t*>(glMapNamedBufferRange(m_name, 0, m_wholeSizeBytes, s_access));
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
    const GLenum target = BufferUsage2GLenum[m_usage];
    if (target != GL_UNIFORM_BUFFER && target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind base of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferBase(target, bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
{
    const GLenum target = BufferUsage2GLenum[m_usage];
    if (target != GL_UNIFORM_BUFFER && target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind range of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferRange(target, bindingIndex, m_name, offsetBytes, sizeBytes);
}

//------------------------------------------------------------------------

void Buffer::freeResources() const noexcept
{
    if (glIsBuffer(m_name) == GL_FALSE) [[unlikely]] return;
    glDeleteBuffers(1, &m_name);
}

//------------------------------------------------------------------------

void Buffer::invalidate(GLintptr offset, GLsizeiptr length) const noexcept
{
    glInvalidateBufferSubData(m_name, offset, length == 0 ? m_writeOffsetBytes : length);
    m_writeOffsetBytes = 0;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
