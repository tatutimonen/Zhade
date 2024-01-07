#pragma once

#include "Buffer.hpp"
#include "Camera.hpp"
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
    CameraDescriptor cameraDesc;
    BufferDescriptor commandBufferDesc{
        .byteSize = MAX_DRAWS * sizeof(DrawElementsIndirectCommand),
        .usage = BufferUsage::INDIRECT
    };
    BufferDescriptor drawMetadataBuffer{
        .byteSize = MAX_DRAWS * sizeof(DrawMetadata),
        .usage = BufferUsage::STORAGE
    };
    PipelineDescriptor mainPassDesc;
};

//------------------------------------------------------------------------

class Renderer
{
public:
    explicit Renderer(RendererDescriptor desc);
    ~Renderer();

    [[nodiscard]] const Camera<CameraType::PERSPECTIVE>& camera() const noexcept { return m_camera; }
    [[nodiscard]] const Scene& scene() const noexcept { return m_scene; }

    void render() const noexcept;

private:
    [[nodiscard]] const Buffer* buffer(const Handle<Buffer>& handle) const noexcept { return m_mngr->get(handle); }
    [[nodiscard]] const Pipeline* pipeline() const noexcept { return m_mngr->get(m_pipeline); }

    void setupVAO() noexcept;
    void setupBuffers(const RendererDescriptor& desc) noexcept;
    void setupCamera(CameraDescriptor cameraDesc) noexcept;
    void setupPipeline(PipelineDescriptor mainPassDesc) noexcept;
    void populateBuffers() const noexcept;
    void clearDrawCounter() const noexcept;

    ResourceManager* m_mngr;
    Scene m_scene;
    GLuint m_vao;
    Camera<CameraType::PERSPECTIVE> m_camera;
    Handle<Buffer> m_commandBuffer;
    Handle<Buffer> m_drawMetadataBuffer;
    Handle<Buffer> m_atomicDrawCounterBuffer;
    Handle<Buffer> m_viewProjUniformBuffer;
    Handle<Pipeline> m_pipeline;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
