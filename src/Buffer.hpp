#pragma once

#include <GL/glew.h>

#include <memory>
#include <utility>

struct BufferLayout {

};

class Buffer {
public:
    Buffer(GLenum gl_buffer_type);
    ~Buffer();

    void bind() const;
    void unbind() const;

    void set_data_store(GLenum buffer_type, GLsizeiptr size, const GLvoid* data, GLenum usage);

    GLuint get_handle() const      { return m_handle; }
    GLenum get_buffer_type() const { return m_buffer_type; }
    GLsizeiptr get_size() const    { return m_size; }
    GLenum get_usage() const       { return m_handle; }

private:
    GLuint m_handle;
    GLenum m_buffer_type;
    GLsizeiptr m_size;
    GLenum m_usage;
    std::shared_ptr<BufferLayout> m_layout;
};