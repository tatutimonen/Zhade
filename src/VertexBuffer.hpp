#pragma once

#include <GL/glew.h>

//------------------------------------------------------------------------

class VertexBuffer {
public:
    VertexBuffer(const GLsizeiptr sizeBytes, const void* data = nullptr, const GLbitfield storageFlags = GL_DYNAMIC_STORAGE_BIT);
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = default;
    VertexBuffer(VertexBuffer&&) = default;
    VertexBuffer& operator=(const VertexBuffer&) = default;
    VertexBuffer& operator=(VertexBuffer&&) = default;

    void setData(const void* data, const GLsizei sizeBytes, const GLintptr offset) const noexcept
    {
        glNamedBufferSubData(m_handle, offset, sizeBytes, data);
    }

private:
    GLuint m_handle;
    GLsizeiptr m_sizeBytes;
    GLbitfield m_storageFlags;
};

//------------------------------------------------------------------------
