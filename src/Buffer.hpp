#pragma once

#include "common.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <robin_hood.h>

#include <cmath>
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
    Buffer(GLenum target, GLsizei sizeBytes, ResourceManagement management);
    ~Buffer();
    
    void freeResources() const noexcept;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    [[nodiscard]] GLuint getGLHandle() const noexcept { return m_handle; }
    [[nodiscard]] GLsizei getWholeSizeBytes() const noexcept { return m_wholeSizeBytes; }

    [[nodiscard]] bool isValid() const noexcept { return m_handle != 0; }

    template<typename T>
    [[nodiscard]] bool fits(GLsizei size) const noexcept
    {
        return m_writeOffsetBytes + sizeof(T)*size <= m_wholeSizeBytes;
    }

    template<typename T>
    std::span<T> pushData(const T* data, GLsizei size) const noexcept
    {
        setData<T>(data, size, m_writeOffsetBytes);
        const GLsizei sizeBytes = sizeof(T) * size;
        const auto span = std::span(mapRangeBump<T>(sizeBytes), size);
        unmap();
        return span;
    }

    template<typename T>
    void setData(const T* data, GLsizei size, GLintptr offsetBytes) const noexcept
    {
        glNamedBufferSubData(m_handle, offsetBytes, sizeof(T) * size, data);
    }

    template<typename T>
    [[nodiscard]] T* map() const noexcept
    {
        return static_cast<T*>(glMapNamedBuffer(m_handle, GL_READ_WRITE));
    }

    template<typename T>
    [[nodiscard]] T* mapRange(GLintptr offsetBytes, GLsizei sizeBytes) const noexcept
    {
        return static_cast<T*>(glMapNamedBufferRange(m_handle, offsetBytes, sizeBytes, s_access));
    }

    template<typename T>
    [[nodiscard]] T* mapRangeBump(GLsizei sizeBytes) const noexcept
    {
        T* ptr = mapRange<T>(m_writeOffsetBytes, sizeBytes);
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeBytes);
        return ptr;
    }

    [[nodiscard]] static robin_hood::unordered_map<GLenum, GLint> makeAlignmentTable() noexcept
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

    void bind() const noexcept;
    void bindBase(GLuint bindingIndex) const noexcept;
    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept;
    void unmap() const noexcept;
    void zero() const noexcept;

    static inline const robin_hood::unordered_map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr computeWriteOffsetIncrement(GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }

    GLuint m_handle;
    GLenum m_target;
    GLsizei m_wholeSizeBytes;
    GLint m_alignment;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
    ResourceManagement m_management = ResourceManagement::MANUAL;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
