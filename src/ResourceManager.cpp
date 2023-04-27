#include "ResourceManager.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

ResourceManager::~ResourceManager()
{
    for (const auto& buffer : m_buffers.m_pool)
        buffer.freeResources();
    for (const auto& tex : m_textures.m_pool)
        tex.freeResources();
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
