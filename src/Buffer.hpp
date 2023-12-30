#pragma once

#include "common.hpp"
#include "util.hpp"

#include <bit>
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
        INDIRECT,
        ATOMIC_COUNTER,
        PARAMETER,
    };
}

inline constexpr GLenum BufferUsage2GLenum[] {
    GL_ARRAY_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_UNIFORM_BUFFER,
    GL_SHADER_STORAGE_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_ATOMIC_COUNTER_BUFFER,
    GL_PARAMETER_BUFFER
};

inline constexpr GLint TBD = 0;

inline GLint BufferUsage2Alignment[] {
    1,
    1,
    TBD,
    TBD,
    1,
    1,
    1
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

    Buffer(const Buffer&) = default;
    Buffer& operator=(const Buffer&) = default;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;

    void freeResources() const noexcept;

    [[nodiscard]] GLuint name() const noexcept { return m_name; }
    [[nodiscard]] GLsizei byteSize() const noexcept { return m_writeOffset; }
    [[nodiscard]] GLsizei wholeByteSize() const noexcept { return m_wholeByteSize; }

    template<typename T>
    [[nodiscard]] T* ptr() const noexcept { return std::bit_cast<T*>(m_ptr); }

    template<typename T>
    [[nodiscard]] T* writePtr() const noexcept { return std::bit_cast<T*>(m_ptr + m_writeOffset); }

    template<typename T>
    [[nodiscard]] size_t size() const noexcept
    {
        return byteSize() / util::roundup(sizeof(T), BufferUsage2Alignment[m_usage]);
    }

    template<typename T>
    std::span<T> pushData(const T* data, GLsizei size = 1) const noexcept
    {
        const GLsizei byteSize = sizeof(T) * size;
        uint8_t* dst = m_ptr + m_writeOffset;
        std::memcpy(dst, data, byteSize);
        m_writeOffset += calculateWriteOffsetIncrement(byteSize);
        return std::span(std::bit_cast<T*>(dst), size);
    }

    template<typename T>
    void setData(const T* data, GLintptr byteOffset = 0, GLsizei size = 1) const noexcept
    {
        glNamedBufferSubData(m_name, byteOffset, sizeof(T) * size, std::bit_cast<const void*>(data));
    }

    void bind() const noexcept;
    void bindAs(BufferUsage::Type usage) const noexcept;
    void bindBase(GLuint bindingIndex) const noexcept;
    void bindBaseAs(GLuint bindingIndex, BufferUsage::Type usage) const noexcept;
    void bindRange(GLuint bindingIndex, GLintptr byteOffset, GLsizeiptr byteSize) const noexcept;
    void invalidate(GLintptr offset = 0, GLsizeiptr length = 0) const noexcept;

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr calculateWriteOffsetIncrement(GLsizei byteSize) const noexcept
    {
        return util::roundup(byteSize, BufferUsage2Alignment[m_usage]);
    }

    GLuint m_name = 0;
    BufferUsage::Type m_usage{};
    GLsizei m_wholeByteSize = 0;
    uint8_t* m_ptr = nullptr;
    mutable GLsizeiptr m_writeOffset = 0;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
