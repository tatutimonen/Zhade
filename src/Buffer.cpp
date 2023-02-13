#include "Buffer.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Buffer::Buffer(GLenum target, GLsizei sizeBytes, common::ResourceManagement management)
    : m_target{target},
      m_wholeSizeBytes{sizeBytes},
      m_management{management},
      m_alignment{s_alignmentTable.at(target)}
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (m_management == common::ResourceManagement::MANUAL)
        return;
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------

void Buffer::freeResources() const noexcept
{
    if (m_management == common::ResourceManagement::RAII) [[unlikely]]
        return;
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------

void Buffer::bind() const noexcept
{
    glBindBuffer(m_target, m_handle);
}

//------------------------------------------------------------------------

void Buffer::bindBase(GLuint bindingIndex) const noexcept
{
    assert(m_target == GL_UNIFORM_BUFFER || m_target == GL_SHADER_STORAGE_BUFFER);
    glBindBufferBase(m_target, bindingIndex, m_handle);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
{
    assert(m_target == GL_UNIFORM_BUFFER || m_target == GL_SHADER_STORAGE_BUFFER);
    glBindBufferRange(m_target, bindingIndex, m_handle, offsetBytes, sizeBytes);
}

//------------------------------------------------------------------------

bool Buffer::fits(GLsizei sizeBytes) const noexcept
{
    return m_writeOffsetBytes + sizeBytes <= m_wholeSizeBytes;
}

//------------------------------------------------------------------------

void Buffer::unmap() const noexcept
{
    glUnmapNamedBuffer(m_handle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
