#include "UniformBufferStorage.hpp"

//------------------------------------------------------------------------

UniformBufferStorage::UniformBufferStorage(const std::shared_ptr<UniformBuffer>& buffer, std::size_t blockOffset)
    : m_buffer{buffer},
        m_blockOffset{blockOffset}
{}

//------------------------------------------------------------------------

void UniformBufferStorage::update(std::size_t offset, const void* data, std::size_t size)
{
    m_buffer->update(offset, data, size, m_blockOffset);
}

//------------------------------------------------------------------------
