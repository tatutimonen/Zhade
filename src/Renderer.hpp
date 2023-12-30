#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Pipeline.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

#include <optional>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RendererDescriptor
{
    ResourceManager* mngr;
    SceneDescriptor sceneDesc;
    BufferDescriptor commandBufferDesc{
        .byteSize = MAX_DRAWS * sizeof(DrawElementsIndirectCommand),
        .usage = BufferUsage::INDIRECT
    };
    BufferDescriptor drawMetadataBuffer{
        .byteSize = MAX_DRAWS * sizeof(DrawMetadata),
        .usage = BufferUsage::STORAGE
    };
    PipelineDescriptor mainPipelineDesc;
};

//------------------------------------------------------------------------

class Renderer
{
public:
    explicit Renderer(RendererDescriptor desc);
    ~Renderer();

    [[nodiscard]] const Scene& scene() const noexcept { return m_scene; }
    [[nodiscard]] const Handle<Buffer>& viewProjBufferHandle() const noexcept { return m_viewProjUniformBuffer; }

    void render() const noexcept;

private:
    [[nodiscard]] const Buffer* buffer(const Handle<Buffer>& handle) const noexcept { return m_mngr->get(handle); }

    void populateBuffers() const noexcept;
    void clearDrawCounter() const noexcept;

    ResourceManager* m_mngr;
    Scene m_scene;
    Pipeline m_mainPipeline;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_drawMetadataBuffer;
    Handle<Buffer> m_atomicDrawCounterBuffer;
    Handle<Buffer> m_viewProjUniformBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
