#pragma once

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class UniformBuffer
{
public:
    UniformBuffer(const GLuint binding, const GLsizeiptr blockSize, const GLsizeiptr extent = 1,
        const GLuint usage = GL_DYNAMIC_DRAW);
    ~UniformBuffer();
    UniformBuffer(const UniformBuffer&) = delete;
    UniformBuffer& operator=(const UniformBuffer&) = delete;
    UniformBuffer(UniformBuffer&&) = default;
    UniformBuffer& operator=(UniformBuffer&&) = default;

    [[nodiscard]] GLuint getHandle() const noexcept { return m_handle; }

    void update(const GLsizeiptr offset, const void* data, const size_t size, const GLsizeiptr blockOffset = 0) const noexcept;
    
    void bindRange(const GLsizeiptr offset, const GLsizei size)
    {
        glBindBufferRange(GL_UNIFORM_BUFFER, m_binding, m_handle, offset, size);
    }

private:
    GLuint m_handle;
    GLuint m_binding;
    GLsizeiptr m_blockSize;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
