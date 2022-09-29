#include "ResourceManager.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

const Buffer* ResourceManager::getBuffer(const Handle<Buffer>& buffer) const noexcept
{
    return m_buffers.get(buffer);
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
