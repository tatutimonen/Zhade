#pragma once

#include "App.hpp"
#include "VertexArray.hpp"

#include <GL/glew.h>

#include <cstdint>
#include <utility>

//------------------------------------------------------------------------

class VertexBuffer {
public:
    VertexBuffer(std::size_t sizeBytes, uint32_t usage, const void* data = nullptr);
    ~VertexBuffer();
    VertexBuffer(const VertexBuffer&) = default;
    VertexBuffer(VertexBuffer&&) = default;
    VertexBuffer& operator=(const VertexBuffer&) = default;
    VertexBuffer& operator=(VertexBuffer&&) = default;

    inline void bind(std::ptrdiff_t stride, uint32_t bindingIdx = 0) const noexcept
    {
        if (!bound())
        {
            glBindVertexBuffer(bindingIdx, m_handle, 0, stride);
            App::getInstance().currentlyBoundBuffer = m_handle;
        }
    }

    inline void setData(const void* data) const noexcept
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        glBufferData(GL_ARRAY_BUFFER, m_sizeBytes, data, usage);
    }

private:
    inline bool bound() const noexcept
    {
        return m_handle == App::getInstance().currentlyBoundBuffer;
    }

    uint32_t m_handle;
    std::size_t m_sizeBytes;
    uint32_t usage;
};

//------------------------------------------------------------------------
