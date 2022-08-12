#pragma once

#include <GL/glew.h>

#include <cmath>
#include <map>
#include <span>

//------------------------------------------------------------------------
// As per https://www.khronos.org/opengl/wiki/Vertex_Rendering#Indirect_rendering.

struct MultiDrawElementsIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

//------------------------------------------------------------------------

template<const GLenum BufferType>
concept IsSupportedGlBufferEnum = (
    BufferType == GL_ARRAY_BUFFER
        || BufferType == GL_ELEMENT_ARRAY_BUFFER
        || BufferType == GL_UNIFORM_BUFFER
        || BufferType == GL_SHADER_STORAGE_BUFFER
        || BufferType == GL_DRAW_INDIRECT_BUFFER
);

//------------------------------------------------------------------------
// Generic OpenGL buffer. Persistently mapped.

template<typename T, const GLenum BufferType>
requires IsSupportedGlBufferEnum<BufferType>
class Buffer
{
public:
    Buffer(const GLsizei wholeSizeBytes = 1 << 27, const GLbitfield accessFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT)
        : m_wholeSizeBytes{wholeSizeBytes},
          m_accessFlags{accessFlags | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT},
          m_alignment{s_alignmentTable.at(BufferType)}
    {
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | m_accessFlags);
    }

    ~Buffer()
    {
        glDeleteBuffers(1, &m_handle);
    }

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = default;

    void setData(const void* data, const GLsizei size, const GLintptr offsetBytes) const noexcept
    {
        glNamedBufferSubData(m_handle, offsetBytes, sizeof(T) * size, data);
    }

    [[nodiscard]] std::span<T> pushData(const void* data, const GLsizei size) const noexcept
    {
        setData(data, size, m_writeOffsetBytes);
        const GLsizei sizeBytes = sizeof(T) * size;
        const auto start = m_writeOffsetBytes;
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeBytes);
        return std::span(mapRange(start, sizeBytes), size);
    }

    void bind() const noexcept
    {
        glBindBuffer(BufferType, m_handle);
    }

    void bindBase(const GLuint bindingIndex) const noexcept
    {
        glBindBufferBase(BufferType, bindingIndex, m_handle);
    }

    [[nodiscard]] T* map() const noexcept
    {
        return static_cast<T*>(glMapNamedBuffer(m_handle, m_accessFlags));
    }

    [[nodiscard]] T* mapRange(const GLintptr offsetBytes, const GLsizei size) const noexcept
    {
        return static_cast<T*>(glMapNamedBufferRange(m_handle, offsetBytes, size, m_accessFlags));
    }

    [[nodiscard]] GLuint getHandle() const noexcept
    {
        return m_handle;
    }

    [[nodiscard]] bool fits(const GLsizei sizeBytes) const noexcept
    {
        return m_writeOffsetBytes + sizeBytes <= m_wholeSizeBytes;
    }

    // GL functions are C and thus not constexpr-compliant, which means alignment info is unavailable at compile-time.

    [[nodiscard]] static std::map<GLenum, GLint> makeAlignmentTable() noexcept
    {
        auto table = std::map<GLenum, GLint>{
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

    static inline const std::map<GLenum, GLint> s_alignmentTable = makeAlignmentTable();

private:
    [[nodiscard]] GLsizeiptr computeWriteOffsetIncrement(const GLsizei sizeBytes) const noexcept
    {
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / m_alignment) * m_alignment);
    }

    GLuint m_handle;
    const GLsizei m_wholeSizeBytes;
    const GLbitfield m_accessFlags;
    const GLint m_alignment;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
};

//------------------------------------------------------------------------
