#pragma once

#include "common.hpp"

#include <glm/glm.hpp>
#include <robin_hood.h>
extern "C" {
#include <GL/glew.h>
}

#include <bit>
#include <cmath>
#include <cstring>
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

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    [[nodiscard]] GLuint getGLHandle() const noexcept { return m_handle; }
    [[nodiscard]] GLsizei getSizeBytes() const noexcept { return m_writeOffsetBytes; }
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
    void pushDataMapped(const T* data, GLsizei size) const noexcept
    {
        T* ptr = std::bit_cast<T*>(m_ptr);  // 256, 8*4 = 32 bytes
        std::memcpy(ptr, data, sizeof(T) * size);
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
    [[nodiscard]] T* mapRangeWhole() const noexcept
    {
        return static_cast<T*>(glMapNamedBufferRange(m_handle, 0, m_wholeSizeBytes, s_access));
    }

    template<typename T>
    [[nodiscard]] T* mapRangeBump(GLsizei sizeBytes) const noexcept
    {
        T* ptr = mapRange<T>(m_writeOffsetBytes, sizeBytes);
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeBytes);
        return ptr;
    }

    void bind() const noexcept;
    void bindBase(GLuint bindingIndex) const noexcept;
    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept;
    void invalidate(GLintptr offset = 0, GLsizeiptr length = 0) const noexcept;
    void unmap() const noexcept;

    [[nodiscard]] static robin_hood::unordered_map<GLenum, GLint> makeAlignmentTable() noexcept;

    // This table is amended with UBO and SSBO information by App upon initialization of the GL context.
    static inline robin_hood::unordered_map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();
    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr computeWriteOffsetIncrement(GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }

    void freeResources() const noexcept;

    GLuint m_handle;
    GLenum m_target;
    GLsizei m_wholeSizeBytes;
    GLint m_alignment;
    void* m_ptr;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
    ResourceManagement m_management = ResourceManagement::MANUAL;

    friend class ResourceManager;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
