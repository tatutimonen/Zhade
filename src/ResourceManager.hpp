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
    [[nodiscard]] Handle<Model2> createModel2(ModelDescriptor desc) { return m_models2.allocate(desc); }
    [[nodiscard]] Handle<Texture> createTexture(TextureDescriptor desc) { return m_textures.allocate(desc); }

    template<typename... Args>
    requires std::constructible_from<Model, Args...>
    [[nodiscard]] Handle<Model> createModel(Args&& ...args)
    {
        return m_models.allocate(std::forward<Args>(args)...);
    }

    [[nodiscard]] Buffer* get(const Handle<Buffer>& handle) const noexcept { return m_buffers.get(handle); }
    [[nodiscard]] Framebuffer* get(const Handle<Framebuffer>& handle) const noexcept { return m_framebuffers.get(handle); }
    [[nodiscard]] Mesh* get(const Handle<Mesh>& handle) const noexcept { return m_meshes.get(handle); }
    [[nodiscard]] Model* get(const Handle<Model>& handle) const noexcept { return m_models.get(handle); }
    [[nodiscard]] Model2* get(const Handle<Model2>& handle) const noexcept { return m_models2.get(handle); }
    [[nodiscard]] Texture* get(const Handle<Texture>& handle) const noexcept { return m_textures.get(handle); }

    void destroy(const Handle<Buffer>& handle) const noexcept { m_buffers.deallocate(handle); }
    void destroy(const Handle<Framebuffer>& handle) const noexcept { m_framebuffers.deallocate(handle); }
    void destroy(const Handle<Mesh>& handle) const noexcept { m_meshes.deallocate(handle); }
    void destroy(const Handle<Model>& handle) const noexcept { m_models.deallocate(handle); }
    void destroy(const Handle<Model2>& handle) const noexcept { m_models2.deallocate(handle); }
    void destroy(const Handle<Texture>& handle) const noexcept { m_textures.deallocate(handle); }

private:
    ObjectPool<Buffer> m_buffers;
    ObjectPool<Framebuffer> m_framebuffers;
    ObjectPool<Mesh> m_meshes;
    ObjectPool<Model> m_models;
    ObjectPool<Model2> m_models2;
    ObjectPool<Texture> m_textures;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
