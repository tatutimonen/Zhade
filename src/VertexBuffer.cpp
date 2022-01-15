#include "VertexBuffer.hpp"

//------------------------------------------------------------------------

VertexBuffer::VertexBuffer(std::size_t sizeBytes, uint32_t usage, const void* data = nullptr)
{
    glGenBuffers(1, &m_handle);
    setData(data);
}

//------------------------------------------------------------------------

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_handle);
}

//------------------------------------------------------------------------
