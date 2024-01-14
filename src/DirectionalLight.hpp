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
    glm::vec3 position{1600.0f, 5300.0f, 920.0f};
    glm::vec3 target{};
    glm::ivec2 shadowMapDims;
    PipelineDescriptor shadowPassDesc;
};

//------------------------------------------------------------------------

class DirectionalLight
{
public:
    explicit DirectionalLight(DirectionalLightDescriptor desc);
    ~DirectionalLight();

    DirectionalLight(const DirectionalLight&) = default;
    DirectionalLight& operator=(const DirectionalLight&) = default;
    DirectionalLight(DirectionalLight&&) = delete;
    DirectionalLight& operator=(DirectionalLight&&) = delete;

    [[nodiscard]] const glm::vec3& direction() const noexcept { return m_props.direction; }

    void prepareForRendering(const Handle<Buffer>& viewProjUniformBuffer) const noexcept;

private:
    [[nodiscard]] const Framebuffer* framebuffer() const noexcept;
    [[nodiscard]] const Buffer* buffer(const Handle<Buffer>& handle) const noexcept;
    [[nodiscard]] const Pipeline* pipeline() const noexcept;

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
