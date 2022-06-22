#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

//------------------------------------------------------------------------

class UniformBuffer {
public:
    UniformBuffer(const std::string& blockName, uint32_t binding, std::ptrdiff_t blockSize, uint32_t extent = 1,
        uint32_t usage = GL_DYNAMIC_DRAW);
    ~UniformBuffer();
    UniformBuffer(const UniformBuffer&) = default;
    UniformBuffer(UniformBuffer&&) = default;
    UniformBuffer& operator=(const UniformBuffer&) = default;
    UniformBuffer& operator=(UniformBuffer&&) = default;

    inline uint32_t getHandle() const noexcept { return m_handle; }

    void update(std::ptrdiff_t offset, const void* data, std::size_t size, std::ptrdiff_t blockOffset = 0) const noexcept;
    inline void bindRange(const GLsizeiptr offset, const GLsizei size)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, m_binding, m_handle, offset, size);
    }

private:
    uint32_t m_handle;
    GLuint m_binding;
    std::ptrdiff_t m_blockSize;
};

//------------------------------------------------------------------------
