#pragma once

#include "common.hpp"

#include <glm/glm.hpp>
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

namespace BufferUsage
{
    using Type = uint8_t;
    enum : Type
    {
        VERTEX,
        INDEX,
        UNIFORM,
        STORAGE,
        INDIRECT
    };
}

inline constexpr GLenum BufferUsage2GLenum[] {
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_SHADER_STORAGE_BUFFER,
    GL_DRAW_INDIRECT_BUFFER
};

inline constexpr GLint TBD = 0;

inline GLint BufferUsage2Alignment[] {
    1,
    1,
    TBD,
    TBD,
    static_cast<GLint>(sizeof(DrawElementsIndirectCommand)),
};

struct BufferDescriptor
{
    GLsizei byteSize;
    BufferUsage::Type usage;
    bool managed = true;
};

//------------------------------------------------------------------------

class Buffer
{
public:
    Buffer() = default;
    explicit Buffer(BufferDescriptor desc);
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(Buffer&&) = default;

    [[nodiscard]] GLuint getName() const noexcept { return m_name; }
    [[nodiscard]] GLsizei getByteSize() const noexcept { return m_writeByteOffset; }
    [[nodiscard]] GLsizei getWholeByteSize() const noexcept { return m_wholeByteSize; }

    template<typename T>
    [[nodiscard]] T* getPtr() const noexcept { return std::bit_cast<T*>(m_ptr); }

    template<typename T>
    [[nodiscard]] T* getWritePtr() const noexcept { return std::bit_cast<T*>(m_ptr + m_writeByteOffset); }

    template<typename T>
    [[nodiscard]] bool fits(GLsizei size) const noexcept
    {
        return m_writeByteOffset + sizeof(T)*size <= m_wholeByteSize;
    }

    template<typename T>
    std::span<T> pushData(const T* data, GLsizei size = 1) const noexcept
    {
        const GLsizei byteSize = sizeof(T) * size;
        uint8_t* dst = m_ptr + m_writeByteOffset;
        std::memcpy(dst, data, byteSize);
        m_writeByteOffset += calculateWriteOffsetIncrement(byteSize);
        return std::span(std::bit_cast<T*>(dst), size);
    }

    template<typename T>
    void setData(const T* data, GLintptr offsetBytes = 0, GLsizei size = 1) const noexcept
    {
        glNamedBufferSubData(m_name, offsetBytes, sizeof(T) * size, data);
    }

    void bind() const noexcept;
    void bindBase(GLuint bindingIndex) const noexcept;
    void bindRange(GLuint bindingIndex, GLintptr offsetBytes, GLsizeiptr byteSize) const noexcept;
    void freeResources() const noexcept;
    void invalidate(GLintptr offset = 0, GLsizeiptr length = 0) const noexcept;

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr calculateWriteOffsetIncrement(GLsizei byteSize) const noexcept
    {
        const GLint alignment = BufferUsage2Alignment[m_usage];
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(byteSize) / alignment) * alignment);
    }

    GLuint m_name = 0;
    BufferUsage::Type m_usage{};
    GLsizei m_wholeByteSize = 0;
    uint8_t* m_ptr = nullptr;
    mutable GLsizeiptr m_writeByteOffset = 0;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
