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

    void update(std::ptrdiff_t offset, const void* data, std::size_t size, std::ptrdiff_t blockOffset = 0) const noexcept;

private:
    uint32_t m_handle;
    std::ptrdiff_t m_blockSize;
};

//------------------------------------------------------------------------
