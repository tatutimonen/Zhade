#pragma once

#include <GL/glew.h>

#include <cstring>
#include <string>
#include <iostream>

//------------------------------------------------------------------------

class UniformBuffer {
public:
    UniformBuffer(const std::string& blockName, GLuint binding, GLsizeiptr size, GLenum usage);
    ~UniformBuffer();

    void update(std::size_t offset, const void* data, std::size_t size);

private:
    GLuint m_handle;
};

//------------------------------------------------------------------------
