#pragma once

#include "Buffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ObjectPool.hpp"
#include "Texture.hpp"
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

    template<typename... Args>
    requires std::constructible_from<Texture, Args..., ResourceManagement>
    [[nodiscard]] Handle<Texture> createTexture(Args&& ...args)
    {
        return m_textures.allocate(std::forward<Args>(args)..., ResourceManagement::MANUAL);
    }

    template<typename T>
    [[nodiscard]] T* get(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            return m_buffers.get(handle);
        else if (std::same_as<T, Model>)
            return m_models.get(handle);
        /*else if (std::same_as<T, Texture>)
            return m_textures.get(handle);*/
        else
            return nullptr;
    }

    template<typename T>
    void destroy(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            m_buffers.deallocate(handle);
        else if (std::same_as<T, Model>)
            m_models.deallocate(handle);
        /*else if (std::same_as<T, Texture>)
            m_textures.deallocate(handle);*/
    }

private:
    ObjectPool<Buffer> m_buffers;
    ObjectPool<Model> m_models;
    ObjectPool<Texture> m_textures;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
