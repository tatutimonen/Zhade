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

    void freeResources();

    [[nodiscard]] GLuint name() { return m_name; }
    [[nodiscard]] GLsizei byteSize() { return m_writeOffset; }
    [[nodiscard]] GLsizei wholeByteSize() { return m_wholeByteSize; }

    template<typename T>
    [[nodiscard]] T* ptr() { return std::bit_cast<T*>(m_ptr); }

    template<typename T>
    [[nodiscard]] T* writePtr() { return std::bit_cast<T*>(m_ptr + m_writeOffset); }

    template<typename T>
    [[nodiscard]] size_t size()
    {
        return byteSize() / util::roundup(sizeof(T), BufferUsage2Alignment[m_usage]);
    }

    template<typename T>
    std::span<T> pushData(const T* data, GLsizei size = 1)
    {
        uint8_t* dst = m_ptr + m_writeOffset;
        const GLsizei byteSize = sizeof(T) * size;
        std::memcpy(dst, data, byteSize);
        m_writeOffset += calculateWriteOffsetIncrement(byteSize);
        return std::span{std::bit_cast<T*>(dst), implicit_cast<size_t>(size)};
    }

    template<typename T>
    void setData(const T* data, GLintptr byteOffset = 0, GLsizei size = 1)
    {
        glNamedBufferSubData(m_name, byteOffset, sizeof(T) * size, std::bit_cast<const void*>(data));
    }

    void bind();
    void bindAs(BufferUsage::Type usage);
    void bindBase(GLuint bindingIndex);
    void bindBaseAs(GLuint bindingIndex, BufferUsage::Type usage);
    void bindRange(GLuint bindingIndex, GLintptr byteOffset, GLsizeiptr byteSize);
    void invalidate(GLintptr offset = 0, GLsizeiptr length = 0);

    static constexpr GLbitfield s_access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

private:
    [[nodiscard]] GLsizeiptr calculateWriteOffsetIncrement(GLsizei byteSize)
    {
        return util::roundup(byteSize, BufferUsage2Alignment[m_usage]);
    }

    GLuint m_name = 0;
    BufferUsage::Type m_usage{};
    GLsizei m_wholeByteSize = 0;
    uint8_t* m_ptr = nullptr;
    GLsizeiptr m_writeOffset = 0;
    bool m_managed = true;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
