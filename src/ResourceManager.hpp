#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "ObjectPool.hpp"

#include <concepts>
#include <utility>
#include <iostream>

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
    requires std::constructible_from<Buffer, Args...>
    [[nodiscard]] Handle<Buffer> createBuffer(Args&& ...args)
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
