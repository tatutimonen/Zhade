#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <cstring>
#include <memory>
#include <string>

//------------------------------------------------------------------------

class UniformBuffer {
public:
    UniformBuffer(const std::string& blockName,
        GLuint binding,
        GLsizeiptr blockSize,
        uint32_t extent = 1,
        GLenum usage = GL_DYNAMIC_DRAW);
    ~UniformBuffer();

    void update(std::size_t offset, const void* data, std::size_t size, std::size_t blockOffset = 0) const noexcept;

private:
    GLuint m_handle;
    std::ptrdiff_t m_blockSize;
};

//------------------------------------------------------------------------
