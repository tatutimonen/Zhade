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
<<<<<<< HEAD
    [[nodiscard]] const Buffer* buffer(const Handle<Buffer>& handle) const noexcept { return m_mngr->get(handle); }
=======
    [[nodiscard]] const Buffer* commandBuffer() const noexcept { return m_mngr->get(m_commandBuffer); }
    [[nodiscard]] const Buffer* drawMetadataBuffer() const noexcept { return m_mngr->get(m_drawMetadataBuffer); }
    [[nodiscard]] const Buffer* atomicDrawCounterBuffer() const noexcept { return m_mngr->get(m_atomicDrawCounterBuffer); }
    [[nodiscard]] const Buffer* meshBuffer() const noexcept { return m_scene->meshBuffer(); }
>>>>>>> 61fcd7157e2ba21f012052e37935a927eb9e21c7

    void populateBuffers() const noexcept;
    void clearDrawCounter() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    Pipeline m_mainPipeline;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_drawMetadataBuffer;
    Handle<Buffer> m_atomicDrawCounterBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
