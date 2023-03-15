#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
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

    template<typename... Args>
    requires std::constructible_from<Buffer, Args..., ResourceManagement>
    [[nodiscard]] Handle<Buffer> createBuffer(Args&& ...args)
    {
        return m_buffers.allocate(std::forward<Args>(args)..., ResourceManagement::MANUAL);
    }

    template<typename... Args>
    requires std::constructible_from<Model, Args...>
    [[nodiscard]] Handle<Model> createModel(Args&& ...args)
    {
        return m_models.allocate(std::forward<Args>(args)...);
    }

    template<typename T>
    [[nodiscard]] const T* get(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            return m_buffers.get(handle);
        else if (std::same_as<T, Model>)
            return m_models.get(handle);

        return nullptr;
    }

    template<typename T>
    void destroy(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            return m_buffers.deallocate(handle);
        else if (std::same_as<T, Model>)
            return m_models.deallocate(handle);
    }

private:
    ObjectPool<Buffer> m_buffers;
    ObjectPool<Model> m_models;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
