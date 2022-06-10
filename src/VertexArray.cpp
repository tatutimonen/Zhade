#include "VertexArray.hpp"

#include "Shader.hpp"

//------------------------------------------------------------------------

VertexArray::VertexArray(const VertexFormat& layout)
{
    glCreateVertexArrays(1, &m_handle);

    GLuint attribIdx = 0;
    for (const auto& [type, sizeBytes, offset, packed] : layout.getElements())
    {
        glEnableVertexArrayAttrib(m_handle, attribIdx);
        glVertexArrayAttribFormat(m_handle,
                                  attribIdx,
                                  Shader::getDataTypeNumElements(type),
                                  Shader::getDataTypeAsGlBaseType(type),
                                  packed ? GL_TRUE : GL_FALSE,
                                  offset);
        glVertexArrayAttribBinding(m_handle, attribIdx++, 0);
    }
}

//------------------------------------------------------------------------

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_handle);
}

//------------------------------------------------------------------------
