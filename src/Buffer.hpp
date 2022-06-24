#pragma once

#include <GL/glew.h>

#include <cassert>
#include <cmath>
#include <map>
#include <span>

namespace sm
{

//------------------------------------------------------------------------
// As per specification: https://www.khronos.org/opengl/wiki/Vertex_Rendering#Indirect_rendering

struct MultiDrawElementsIndirectCommand
{
    GLuint vertexCount;
    GLuint instanceCount;
    GLuint firstIndex;
    GLuint baseVertex;
    GLuint baseInstance;
};

//------------------------------------------------------------------------

template <typename T>
class Buffer
{
public:
    Buffer(const GLenum target, const GLsizei wholeSizeBytes = 1 << 20, const GLbitfield accessFlags = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT)
        : m_wholeSizeBytes{wholeSizeBytes},
          m_accessFlags{accessFlags}
    {
        assert(target == GL_ARRAY_BUFFER
            || target == GL_ELEMENT_ARRAY_BUFFER
            || target == GL_UNIFORM_BUFFER
            || target == GL_SHADER_STORAGE_BUFFER
            || target == GL_DRAW_INDIRECT_BUFFER);
        m_target = target;
        glCreateBuffers(1, &m_handle);
        glNamedBufferStorage(m_handle, m_wholeSizeBytes, nullptr, GL_DYNAMIC_STORAGE_BIT | accessFlags);
    }

    ~Buffer()
    {
        unmap();
        glDeleteBuffers(1, &m_handle);
    }

    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = default;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) = default;

    inline void setData(const void* data, const GLsizei size, const GLintptr offsetBytes) const noexcept
    {
        glNamedBufferSubData(m_handle, offsetBytes, sizeof(T) * size, data);
    }

    [[nodiscard]]
    inline std::span<T> pushData(const void* data, const GLsizei size) const noexcept
    {
        setData(data, size, m_writeOffsetBytes);
        const auto start = m_writeOffsetBytes;
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeof(T) * size);
        const auto end = m_writeOffsetBytes - start;
        auto buffer = static_cast<T*>(mapRange(start, sizeof(T) * size));
        return std::span<T>(buffer, end);
    }

    inline void pushData(const void* data, const GLsizei size) const noexcept
    {
        setData(data, size, m_writeOffsetBytes);
        m_writeOffsetBytes += computeWriteOffsetIncrement(sizeof(T) * size);
    }

    inline void bindToTarget() const noexcept
    {
        glBindBuffer(m_target, m_handle);
    }

    inline void bindBase(const GLuint bindingIndex) const noexcept
    {
        glBindBufferBase(m_target, bindingIndex, m_handle);
    }

    inline void* map() const noexcept
    {
        return glMapNamedBuffer(m_handle, m_accessFlags);
    }

    inline void* mapRange(const GLintptr offsetBytes, const GLsizei size) const noexcept
    {
        return glMapNamedBufferRange(m_handle, offsetBytes, size, m_accessFlags);
    }

    void unmap() const noexcept
    {
        glUnmapNamedBuffer(m_handle);
    }

    inline GLuint getHandle() const noexcept
    {
        return m_handle;
    }

    inline bool fits(const GLsizei sizeBytes) const noexcept
    {
        return m_writeOffsetBytes + sizeBytes <= m_wholeSizeBytes;
    }

    static inline std::map<GLenum, GLint> makeAlignmentTable() noexcept
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
    GLsizeiptr computeWriteOffsetIncrement(const GLsizei sizeBytes) const noexcept
    {
        const auto alignment = s_alignmentTable.at(m_target);
        return static_cast<GLsizeiptr>(std::ceil(static_cast<float>(sizeBytes) / alignment) * alignment);
    }

    GLenum m_target;
    GLuint m_handle;
    GLsizei m_wholeSizeBytes;
    GLbitfield m_accessFlags;
    mutable GLsizeiptr m_writeOffsetBytes = 0;
};

//------------------------------------------------------------------------

}  // namespace sm
