#include "VertexArray.hpp"


VertexArray::VertexArray()
{
    GL_CALL(glGenVertexArrays(1, &m_handle));
}

VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_handle));
}

void VertexArray::bind() const
{
    GL_CALL(glBindVertexArray(m_handle));
}

void VertexArray::unbind() const
{
    GL_CALL(glBindVertexArray(0));
}

void VertexArray::add_buffer(std::shared_ptr<Buffer> bfr)
{
    
}

void VertexArray::draw_buffers() const 
{

}