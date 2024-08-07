#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Pipeline.hpp"
#include "common.hpp"

#include <glm/glm.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct DirectionalLightDescriptor
{
    ResourceManager* mngr;
    DirectionalLightProperties props;
    glm::vec3 position{-1163.729858, 4203.104980, -258.124634};
    glm::ivec2 shadowMapDims;
    PipelineDescriptor shadowPassDesc;
};

//------------------------------------------------------------------------

class DirectionalLight
{
public:
    explicit DirectionalLight(DirectionalLightDescriptor desc);
    ~DirectionalLight();

    DirectionalLight(const DirectionalLight&) = delete;
    DirectionalLight& operator=(const DirectionalLight&) = delete;
    DirectionalLight(DirectionalLight&&) = delete;
    DirectionalLight& operator=(DirectionalLight&&) = delete;

    [[nodiscard]] const glm::vec3& direction() { return m_props.direction; }

    void prepareForRendering(const Handle<Buffer>& viewProjUniformBuffer);

private:
    [[nodiscard]] Framebuffer* framebuffer();
    [[nodiscard]] Buffer* buffer(const Handle<Buffer>& handle);
    [[nodiscard]] Pipeline* pipeline();

    void setupMatrices(const DirectionalLightDescriptor& desc);
    void setupFramebuffer();
    void setupBuffers();

    ResourceManager* m_mngr;
    DirectionalLightProperties m_props;
    glm::ivec2 m_shadowMapDims;
    ViewProjMatrices m_matrices;
    Handle<Framebuffer> m_framebuffer;
    Handle<Buffer> m_propsBuffer;
    Handle<Buffer> m_depthTextureBuffer;
    Handle<Buffer> m_shadowMatrixBuffer;
    Handle<Pipeline> m_pipeline;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
