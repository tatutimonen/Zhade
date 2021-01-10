#pragma once

#include "gl_common.hpp"
#include <Buffer.hpp>

#include <GL/glew.h>

#include <utility>
#include <vector>


class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    GLuint get_handle() const { return m_handle; }

    void bind() const;
    void unbind() const;
    void add_buffer(std::shared_ptr<Buffer> bfr);
    void draw_buffers() const;

private:
    GLuint m_handle;
    std::vector<std::shared_ptr<Buffer>> m_vertex_buffers;
    std::shared_ptr<Buffer> m_element_buffer;
};