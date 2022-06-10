#pragma once

#include "VertexFormat.hpp"

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

class VertexArray {
public:
    VertexArray(const VertexFormat& layout);
    ~VertexArray();
    VertexArray(const VertexArray&) = default;
    VertexArray(VertexArray&&) = default;
    VertexArray& operator=(const VertexArray&) = default;
    VertexArray& operator=(VertexArray&&) = default;

    // Binding still required for drawing.
    void bind() const noexcept { glBindVertexArray(m_handle); }

private:
    GLuint m_handle;
};

//------------------------------------------------------------------------
