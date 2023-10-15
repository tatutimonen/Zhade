#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Mesh.hpp"
#include "Model.hpp"
#include "ObjectPool.hpp"
#include "Texture.hpp"
#include "common.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept ManagedType = (
    std::same_as<T, Buffer>
    or std::same_as<T, Framebuffer>
    or std::same_as<T, Mesh>
    or std::same_as<T, Model>
    or std::same_as<T, Texture>
);

//------------------------------------------------------------------------
// Inspired by: https://twitter.com/SebAaltonen/status/1535175559067713536.

class ResourceManager
{
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    [[nodiscard]] Handle<Buffer> createBuffer(BufferDescriptor desc) { return m_buffers.allocate(desc); }
    [[nodiscard]] Handle<Framebuffer> createFramebuffer(FramebufferDescriptor desc) { return m_framebuffers.allocate(desc); }
    [[nodiscard]] Handle<Mesh> createMesh(MeshDescriptor desc) { return m_meshes.allocate(desc); }
    [[nodiscard]] Handle<Model> createModel(ModelDescriptor desc) { return m_models.allocate(desc); }
    [[nodiscard]] Handle<Texture> createTexture(TextureDescriptor desc) { return m_textures.allocate(desc); }

    template<ManagedType T>
    [[nodiscard]] T* get(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            return m_buffers.get(handle);
        else if constexpr (std::same_as<T, Framebuffer>)
            return m_framebuffers.get(handle);
        else if constexpr (std::same_as<T, Mesh>)
            return m_meshes.get(handle);
        else if constexpr (std::same_as<T, Model>)
            return m_models.get(handle);
        else if constexpr (std::same_as<T, Texture>)
            return m_textures.get(handle);
    }

    template<ManagedType T>
    void destroy(const Handle<T>& handle) const noexcept
    {
        if constexpr (std::same_as<T, Buffer>)
            m_buffers.deallocate(handle);
        else if constexpr (std::same_as<T, Framebuffer>)
            m_framebuffers.deallocate(handle);
        else if constexpr (std::same_as<T, Mesh>)
            m_meshes.deallocate(handle);
        else if constexpr (std::same_as<T, Model>)
            m_models.deallocate(handle);
        else if constexpr (std::same_as<T, Texture>)
            m_textures.deallocate(handle);
    }

private:
    ObjectPool<Buffer> m_buffers;
    ObjectPool<Framebuffer> m_framebuffers;
    ObjectPool<Mesh> m_meshes;
    ObjectPool<Model> m_models;
    ObjectPool<Texture> m_textures;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
