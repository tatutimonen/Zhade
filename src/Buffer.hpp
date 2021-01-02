#pragma once

#include <GL/glew.h>


class Buffer {
public:
    Buffer();
    ~Buffer();

    void bind() const;
    void unbind() const;

    void set_data_store();


private:
    GLuint m_handle;
    GLenum m_buffer_type;
    GLsizeiptr m_size;
    GLenum m_draw_type;
};