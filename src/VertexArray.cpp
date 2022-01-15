#include "VertexArray.hpp"

//------------------------------------------------------------------------

VertexArray::VertexArray(std::initializer_list<FormatSpecifier> format)
{
    glGenVertexArrays(1, &m_handle);
    glBindVertexArray(m_handle);

    for (const auto [location, sizeBytes, type, normalized, offset] : format)
    {
        glEnableVertexAttribArray(location);
        glVertexAttribFormat(location, sizeBytes, type, normalized, offset);
        glVertexAttribBinding(location, 0);
    }

    glBindVertexArray(0);
}

//------------------------------------------------------------------------

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_handle);
}

//------------------------------------------------------------------------
