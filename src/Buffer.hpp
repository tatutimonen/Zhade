#pragma once

#include "Renderer.hpp"

#include <GL/glew.h>

#include <cmath>
#include <map>
#include <optional>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<GLenum BufferType>
concept SupportedGlBufferEnum = (
    BufferType == GL_ARRAY_BUFFER
        || BufferType == GL_ELEMENT_ARRAY_BUFFER
        || BufferType == GL_UNIFORM_BUFFER
        || BufferType == GL_SHADER_STORAGE_BUFFER
        || BufferType == GL_DRAW_INDIRECT_BUFFER
);

//------------------------------------------------------------------------
// Generic OpenGL buffer, persistently and coherently mapped.

template<typename T, GLenum BufferType>
requires SupportedGlBufferEnum<BufferType>
class Buffer
{
public:
    Buffer(GLsizei sizeBytes = 1 << 27)
        : m_wholeSizeBytes{sizeBytes},
          m_alignment{s_alignmentTable.at(BufferType)}
    {
        glCreateBuffers(1, &m_handle);
        static constexpr GLbitfield access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | access);
    }

    ~Buffer()
    {
        glDeleteBuffers(1, &m_handle);
    }

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    [[nodiscard]] std::span<T> pushData(const void* data, GLsizei size) const noexcept
    {
        setData(data, size, m_writeOffsetBytes);
        const GLsizeiptr start = m_writeOffsetBytes;
        const GLsizei sizeBytes = sizeof(T) * size;
        T* dataPtr = mapRange(start, sizeBytes);
        const auto span = std::span(dataPtr, size);
        unmap();
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeBytes);
        return span;
    }

    void setData(const void* data, GLsizei size, GLintptr offsetBytes) const noexcept
    {
        glNamedBufferSubData(m_handle, offsetBytes, sizeof(T) * size, data);
    }

    void bind() const noexcept
    {
        glBindBuffer(BufferType, m_handle);
    }

    void bindBase(GLuint bindingIndex) const noexcept
    requires (BufferType == GL_UNIFORM_BUFFER || BufferType == GL_SHADER_STORAGE_BUFFER)
    {
        glBindBufferBase(BufferType, bindingIndex, m_handle);
    }

    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
    requires (BufferType == GL_UNIFORM_BUFFER || BufferType == GL_SHADER_STORAGE_BUFFER)
    {
        glBindBufferRange(BufferType, bindingIndex, m_handle, offsetBytes, sizeBytes);
    }

    [[nodiscard]] T* map() const noexcept
    {
        return static_cast<T*>(glMapNamedBuffer(m_handle, GL_READ_WRITE));
    }

    [[nodiscard]] T* mapRange(GLintptr offsetBytes, GLsizei sizeBytes) const noexcept
    {
        return static_cast<T*>(glMapNamedBufferRange(m_handle, offsetBytes, sizeBytes, GL_READ_WRITE));
    }

    void unmap() const noexcept
    {
        glUnmapNamedBuffer(m_handle);
    }

    [[nodiscard]] GLuint getHandle() const noexcept
    {
        return m_handle;
    }

    [[nodiscard]] bool fits(GLsizei sizeBytes) const noexcept
    {
        return m_writeOffsetBytes + sizeBytes <= m_wholeSizeBytes;
    }

    // GL functions are C and thus not constexpr-compliant, which means alignment info is unavailable at compile-time.

    [[nodiscard]] static std::map<GLenum, GLint> makeAlignmentTable() noexcept
    {
        auto table = std::map<GLenum, GLint>{
            { GL_ARRAY_BUFFER, 1 },
            { GL_ELEMENT_ARRAY_BUFFER, 1 },
            { GL_DRAW_INDIRECT_BUFFER, static_cast<GLint>(sizeof(Zhade::MultiDrawElementsIndirectCommand)) }
        };

        GLint uniformBufferAlignment;
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferAlignment);
        table.insert({ GL_UNIFORM_BUFFER, uniformBufferAlignment });

        GLint storageBufferAlignment;
        glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &storageBufferAlignment);
        table.insert({ GL_SHADER_STORAGE_BUFFER, storageBufferAlignment });

        return table;
    }

    static inline const std::map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();

private:
    [[nodiscard]] GLsizeiptr computeWriteOffsetIncrement(GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }


    GLuint m_handle;
    const GLsizei m_wholeSizeBytes;
    const GLint m_alignment;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
