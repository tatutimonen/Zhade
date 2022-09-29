#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "ObjectPool.hpp"

#include <type_traits>
#include <utility>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Reference: https://twitter.com/SebAaltonen/status/1535175559067713536.

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = default;
    ResourceManager& operator=(ResourceManager&&) = default;

    template<typename... Args>
    requires std::is_constructible_v<Buffer, Args...>
    [[nodiscard]] Handle<Buffer> createBuffer(Args&& ...args) const
    {
        return m_buffers.allocate(std::forward<Args>(args)...);
    }

    [[nodiscard]] const Buffer* getBuffer(const Handle<Buffer>& buffer) const noexcept;

private:
    ObjectPool<Buffer> m_buffers;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
