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
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
}

//------------------------------------------------------------------------

Buffer::~Buffer()
{
    if (m_management == ResourceManagement::MANUAL) [[likely]] return;
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
    if (m_target != GL_UNIFORM_BUFFER && m_target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind base of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferBase(m_target, bindingIndex, m_handle);
}

//------------------------------------------------------------------------

void Buffer::bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
{
    if (m_target != GL_UNIFORM_BUFFER && m_target != GL_SHADER_STORAGE_BUFFER) [[unlikely]]
    {
        std::cerr << "Bind range of non-UBO or non-SSBO\n";
        return;
    }
    glBindBufferRange(m_target, bindingIndex, m_handle, offsetBytes, sizeBytes);
}

//------------------------------------------------------------------------

void Buffer::invalidate(GLintptr offset, GLsizeiptr length) const noexcept
{
    glInvalidateBufferSubData(m_handle, offset, length == 0 ? m_writeOffsetBytes : length);
    m_writeOffsetBytes = 0;
}

//------------------------------------------------------------------------

void Buffer::unmap() const noexcept
{
    glUnmapNamedBuffer(m_handle);
}

//------------------------------------------------------------------------

robin_hood::unordered_map<GLenum, GLint> Buffer::makeAlignmentTable() noexcept
{
    auto table = robin_hood::unordered_map<GLenum, GLint>{
        { GL_ARRAY_BUFFER, 1 },
        { GL_ELEMENT_ARRAY_BUFFER, 1 },
        { GL_DRAW_INDIRECT_BUFFER, static_cast<GLint>(sizeof(MultiDrawElementsIndirectCommand)) }
    };

    GLint uniformBufferAlignment;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferAlignment);
    table.insert({ GL_UNIFORM_BUFFER, uniformBufferAlignment });

    GLint storageBufferAlignment;
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &storageBufferAlignment);
    table.insert({ GL_SHADER_STORAGE_BUFFER, storageBufferAlignment });

    return table;
}

//------------------------------------------------------------------------

void Buffer::freeResources() const noexcept
{
    if (m_management == ResourceManagement::RAII) [[unlikely]] return;
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
