#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Pipeline.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct RendererDescriptor
{
    ResourceManager* mngr;
    Scene* scene;
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

    void render() const noexcept;

private:
    [[nodiscard]] const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }
    [[nodiscard]] const Buffer* drawMetadataBuffer() const noexcept { return m_mngr->get(m_drawMetadataBuffer); }
    [[nodiscard]] const Buffer* atomicDrawCounterBuffer() const noexcept { return m_mngr->get(m_atomicDrawCounterBuffer); }
    [[nodiscard]] const Buffer* parameterBuffer() const noexcept { return m_mngr->get(m_parameterBuffer); }
    [[nodiscard]] const Buffer* meshBuffer() const noexcept { return m_scene->meshBuffer(); }

    void populateBuffers() const noexcept;
    void clearDrawCounter() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    Pipeline m_mainPipeline;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_drawMetadataBuffer;
    Handle<Buffer> m_atomicDrawCounterBuffer;
    Handle<Buffer> m_parameterBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
