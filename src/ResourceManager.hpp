#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "ObjectPool.hpp"
#include "Pipeline.hpp"
#include "Texture.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

template<typename T>
concept ManagedType = (
    std::same_as<T, Buffer>
    or std::same_as<T, Framebuffer>
    or std::same_as<T, Model>
    or std::same_as<T, Pipeline>
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

    [[nodiscard]] Handle<Buffer> createBuffer(BufferDescriptor desc)
    {
        return m_buffers.allocate(desc);
    }
    
    [[nodiscard]] Handle<Framebuffer> createFramebuffer(FramebufferDescriptor desc)
    {
        return m_framebuffers.allocate(desc);
    }
    
    [[nodiscard]] Handle<Model> createModel(ModelDescriptor desc)
    {
        return m_models.allocate(desc);
    }
    
    [[nodiscard]] Handle<Pipeline> createPipeline(PipelineDescriptor desc)
    {
        return m_pipelines.allocate(desc);
    }
    
    [[nodiscard]] Handle<Texture> createTexture(TextureDescriptor desc)
    {
        return m_textures.allocate(desc);
    }

    template<ManagedType T>
    [[nodiscard]] T* get(const Handle<T>& handle)
    {
        if constexpr (std::same_as<T, Buffer>)
            return m_buffers.get(handle);
        else if constexpr (std::same_as<T, Framebuffer>)
            return m_framebuffers.get(handle);
        else if constexpr (std::same_as<T, Model>)
            return m_models.get(handle);
        else if constexpr (std::same_as<T, Pipeline>)
            return m_pipelines.get(handle);
        else if constexpr (std::same_as<T, Texture>)
            return m_textures.get(handle);
    }

    template<ManagedType T>
    void destroy(const Handle<T>& handle)
    {
        if constexpr (std::same_as<T, Buffer>)
            m_buffers.deallocate(handle);
        else if constexpr (std::same_as<T, Framebuffer>)
            m_framebuffers.deallocate(handle);
        else if constexpr (std::same_as<T, Model>)
            m_models.deallocate(handle);
        else if constexpr (std::same_as<T, Pipeline>)
            m_pipelines.deallocate(handle);
        else if constexpr (std::same_as<T, Texture>)
            m_textures.deallocate(handle);
    }

    template<ManagedType T>
    bool exists(const Handle<T>& handle)
    {
        return get(handle) != nullptr;
    }

private:
    ObjectPool<Buffer> m_buffers;
    ObjectPool<Framebuffer> m_framebuffers;
    ObjectPool<Model> m_models;
    ObjectPool<Pipeline> m_pipelines;
    ObjectPool<Texture> m_textures;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
