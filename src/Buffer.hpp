#pragma once

#include "Renderer.hpp"

#include <GL/glew.h>
#include <robin_hood.h>

#include <cassert>
#include <cmath>
#include <map>
#include <optional>
#include <span>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Generic OpenGL buffer, persistently and coherently mapped.

class Buffer
{
public:
    Buffer() = default;

    Buffer(GLenum target, GLsizei sizeBytes = 1 << 27)
        : m_target{target},
          m_wholeSizeBytes{sizeBytes},
          m_alignment{s_alignmentTable.at(target)}
    {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | s_access);
    }

    ~Buffer() { glDeleteBuffers(1, &m_handle); }

    Buffer(const Buffer&) = default;
    Buffer& operator=(const Buffer&) = default;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }

    [[nodiscard]] bool isValid() const noexcept { return glIsBuffer(m_handle); }

    template<typename T>
    [[nodiscard]] std::span<T> pushData(const void* data, GLsizei size) const noexcept
    {
        setData<T>(data, size, m_writeOffsetBytes);
        const GLsizei sizeBytes = sizeof(T) * size;
        T* dataPtr = mapRange<T>(m_writeOffsetBytes, sizeBytes);
        const auto span = std::span(dataPtr, size);
        unmap();
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeBytes);
        return span;
    }

    template<typename T>
    void setData(const void* data, GLsizei size, GLintptr offsetBytes) const noexcept
    {
        glNamedBufferSubData(m_handle, offsetBytes, sizeof(T) * size, data);
    }

    void bind() const noexcept
    {
        glBindBuffer(m_target, m_handle);
    }

    void bindBase(GLuint bindingIndex) const noexcept
    {
        assert(m_target == GL_UNIFORM_BUFFER || m_target == GL_SHADER_STORAGE_BUFFER);
        glBindBufferBase(m_target, bindingIndex, m_handle);
    }

    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept
    {
        assert(m_target == GL_UNIFORM_BUFFER || m_target == GL_SHADER_STORAGE_BUFFER);
        glBindBufferRange(m_target, bindingIndex, m_handle, offsetBytes, sizeBytes);
    }

    template<typename T>
    [[nodiscard]] T* map() const noexcept
    {
        return static_cast<T*>(glMapNamedBuffer(m_handle, s_access));
    }

    template<typename T>
    [[nodiscard]] T* mapRange(GLintptr offsetBytes, GLsizei sizeBytes) const noexcept
    {
        return static_cast<T*>(glMapNamedBufferRange(m_handle, offsetBytes, sizeBytes, s_access));
    }

    template<typename T>
    [[nodiscard]] T* mapRangeBump(GLsizei sizeBytes) const noexcept
    {
        T* ptr = static_cast<T*>(glMapNamedBufferRange(m_handle, m_writeOffsetBytes, sizeBytes, s_access));
        m_writeOffsetBytes += sizeBytes;
        return ptr;
    }

    void unmap() const noexcept
    {
        glUnmapNamedBuffer(m_handle);
    }

    [[nodiscard]] bool fits(GLsizei sizeBytes) const noexcept
    {
        return m_writeOffsetBytes + sizeBytes <= m_wholeSizeBytes;
    }

    [[nodiscard]] static robin_hood::unordered_map<GLenum, GLint> makeAlignmentTable() noexcept
    {
        auto table = robin_hood::unordered_map<GLenum, GLint>{
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

    static inline const robin_hood::unordered_map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr computeWriteOffsetIncrement(GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }

    GLuint m_handle = 0;
    const GLenum m_target = 0;
    const GLsizei m_wholeSizeBytes = 0;
    const GLint m_alignment = 0;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
