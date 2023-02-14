#include "ResourceManager.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

ResourceManager::~ResourceManager()
{
    for (const auto& buffer : m_buffers.m_pool)
        buffer.freeResources();
}

//------------------------------------------------------------------------

const Buffer* ResourceManager::getBuffer(const Handle<Buffer>& buffer) const noexcept
{
    return m_buffers.get(buffer);
}

//------------------------------------------------------------------------

void ResourceManager::deleteBuffer(const Handle<Buffer>& buffer) const noexcept
{
    m_buffers.deallocate(buffer);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
