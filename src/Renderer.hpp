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
    BufferDescriptor commandBufferDesc{.byteSize = KIB_BYTES * 100, .usage = BufferUsage::INDIRECT};
    BufferDescriptor transformBufferDesc{.byteSize = MAX_DRAWS * sizeof(glm::mat3x4), .usage = BufferUsage::STORAGE};
    BufferDescriptor textureBufferDesc{.byteSize = KIB_BYTES * 100, .usage = BufferUsage::STORAGE};
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
    [[nodiscard]] const Buffer* transformBuffer() const noexcept { return m_mngr->get(m_transformBuffer); }
    [[nodiscard]] const Buffer* textureBuffer() const noexcept { return m_mngr->get(m_textureBuffer); }
    [[nodiscard]] const Buffer* atomicCounterBuffer() const noexcept { return m_mngr->get(m_atomicCounterBuffer); }
    [[nodiscard]] const Buffer* parameterBuffer() const noexcept { return m_mngr->get(m_parameterBuffer); }
    [[nodiscard]] const Buffer* meshBuffer() const noexcept { return m_scene->meshBuffer(); }

    void populateBuffers() const noexcept;

    ResourceManager* m_mngr;
    Scene* m_scene;
    Pipeline m_mainPipeline;
    GLuint m_vao;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_transformBuffer;
    Handle<Buffer> m_textureBuffer;
    Handle<Buffer> m_atomicCounterBuffer;
    Handle<Buffer> m_parameterBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
