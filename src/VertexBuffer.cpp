#include "VertexBuffer.hpp"
#include <iostream>

//------------------------------------------------------------------------

VertexBuffer::VertexBuffer(const GLsizeiptr sizeBytes, const void* data, const GLbitfield storageFlags)
    : m_sizeBytes{sizeBytes}
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, m_sizeBytes, data, storageFlags);
}

//------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------
