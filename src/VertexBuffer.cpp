#include "VertexBuffer.hpp"
#include <iostream>

//------------------------------------------------------------------------

VertexBuffer::VertexBuffer(const GLsizei sizeBytes, const GLbitfield storageFlags)
    : m_sizeBytes{sizeBytes}
{
    glCreateBuffers(1, &m_handle);
    glNamedBufferStorage(m_handle, m_sizeBytes, nullptr, storageFlags);
}

//------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------
