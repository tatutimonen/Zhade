#include "UniformBufferStorage.hpp"

//------------------------------------------------------------------------

void UniformBufferStorage::update(std::size_t offset, const void* data, std::size_t size)
{
    m_buffer->update(offset, data, size, m_blockOffset);
}

//------------------------------------------------------------------------
