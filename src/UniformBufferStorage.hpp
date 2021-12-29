#pragma once

#include "UniformBuffer.hpp"

#include <GL/glew.h>

#include <memory>

//------------------------------------------------------------------------

class UniformBufferStorage {
public:
    UniformBufferStorage(const std::shared_ptr<UniformBuffer>& buffer, std::size_t blockOffset);

    void update(std::size_t offset, const void* data, std::size_t size);

private:
    std::shared_ptr<UniformBuffer> m_buffer;
    std::size_t m_blockOffset;
};

//------------------------------------------------------------------------