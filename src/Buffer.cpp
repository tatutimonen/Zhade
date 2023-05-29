#include "Buffer.hpp"

#include <cstring>
#include <iostream>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Buffer::Buffer(GLenum target, GLsizei sizeBytes, ResourceManagement management)
    : m_target{target},
      m_wholeSizeBytes{sizeBytes},
      m_management{management},
      m_alignment{s_alignmentTable.at(target)}
{
    glCreateBuffers(1, &m_name);
    glNamedBufferStorage(m_name, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
    m_ptr = std::bit_cast<uint8_t*>(glMapNamedBufferRange(m_name, 0, m_wholeSizeBytes, s_access));
    glUnmapNamedBuffer(m_name);
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (m_management == ResourceManagement::MANUAL) [[likely]] return;
    glDeleteBuffers(1, &m_name);
}

//------------------------------------------------------------------------

void Buffer::bind() const noexcept
{
    glBindBuffer(m_target, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindBase(GLuint bindingIndex) const noexcept
{
    if (m_target != GL_UNIFORM_BUFFER && m_target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind base of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferBase(m_target, bindingIndex, m_name);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
{
    if (m_target != GL_UNIFORM_BUFFER && m_target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind range of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferRange(m_target, bindingIndex, m_name, offsetBytes, sizeBytes);
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

robin_hood::unordered_map<GLenum, GLint> Buffer::makeAlignmentTable() noexcept
{
    return robin_hood::unordered_map<GLenum, GLint>{
        { GL_ARRAY_BUFFER, 1 },
        { GL_ELEMENT_ARRAY_BUFFER, 1 },
        { GL_DRAW_INDIRECT_BUFFER, static_cast<GLint>(sizeof(MultiDrawElementsIndirectCommand)) }
    };
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
