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

    [[nodiscard]] GLuint getName() const noexcept { return m_name; }
    [[nodiscard]] GLsizei getSizeBytes() const noexcept { return m_writeOffsetBytes; }
    [[nodiscard]] GLsizei getWholeSizeBytes() const noexcept { return m_wholeSizeBytes; }

    template<typename T>
    [[nodiscard]] T* getPtr() const noexcept { return std::bit_cast<T*>(m_ptr); }

    template<typename T>
    [[nodiscard]] T* getWritePtr() const noexcept { return std::bit_cast<T*>(m_ptr + m_writeOffsetBytes); }

    template<typename T>
    [[nodiscard]] bool fits(GLsizei size) const noexcept
    {
        return m_writeOffsetBytes + sizeof(T)*size <= m_wholeSizeBytes;
    }

    template<typename T>
    std::span<T> pushData(const T* data, GLsizei size = 1) const noexcept
    {
        const GLsizei sizeBytes = sizeof(T) * size;
        uint8_t* dst = m_ptr + m_writeOffsetBytes;
        std::memcpy(dst, data, sizeBytes);
        m_writeOffsetBytes += calculateWriteOffsetIncrement(sizeBytes);
        return std::span(std::bit_cast<T*>(dst), size);
    }

    template<typename T>
    void setData(const T* data, GLintptr offsetBytes = 0, GLsizei size = 1) const noexcept
    {
        glNamedBufferSubData(m_name, offsetBytes, sizeof(T) * size, data);
    }

    void bind() const noexcept;
    void bindBase(GLuint bindingIndex) const noexcept;
    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr sizeBytes) const noexcept;
    void freeResources() const noexcept;
    void invalidate(GLintptr offset = 0, GLsizeiptr length = 0) const noexcept;

    [[nodiscard]] static robin_hood::unordered_map<GLenum, GLint> makeAlignmentTable() noexcept;

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

    // This table is amended with UBO -and SSBO information by App upon initialization of the GL context.
    static inline robin_hood::unordered_map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();

private:
    [[nodiscard]] GLsizeiptr calculateWriteOffsetIncrement(GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }

    GLuint m_name{};
    GLenum m_target{};
    GLsizei m_wholeSizeBytes{};
    GLint m_alignment{};
    uint8_t* m_ptr{};
    mutable GLsizeiptr m_writeOffsetBytes{};
    ResourceManagement m_management{ResourceManagement::MANUAL};
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
