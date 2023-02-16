#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "ObjectPool.hpp"
#include "common.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------
// Inspired by: https://twitter.com/SebAaltonen/status/1535175559067713536.

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = default;
    ResourceManager& operator=(ResourceManager&&) = default;

    template<typename T>
    [[nodiscard]] const T* operator()(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            return getBuffer(handle);

        return nullptr;
    }

    template<typename... Args>
    requires std::constructible_from<Buffer, Args..., ResourceManagement>
    [[nodiscard]] Handle<Buffer> createBuffer(Args&& ...args)
    {
        return m_buffers.allocate(std::forward<Args>(args)..., ResourceManagement::MANUAL);
    }

    [[nodiscard]] const Buffer* getBuffer(const Handle<Buffer>& buffer) const noexcept;
    void deleteBuffer(const Handle<Buffer>& buffer) const noexcept;

private:
    ObjectPool<Buffer> m_buffers;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
