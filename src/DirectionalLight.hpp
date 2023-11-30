#pragma once

#include "Buffer.hpp"
#include "Framebuffer.hpp"
#include "Handle.hpp"
#include "Pipeline.hpp"

#include <glm/glm.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct DirectionalLightDescriptor
{
    DirectionalLightData data;
    glm::ivec2 shadowMapDims;
    PipelineDescriptor shadowPassDesc;
    ResourceManager* mngr;
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

    [[nodiscard]] const glm::vec3& direction() const noexcept { return m_data.direction; }

    void prepareForRendering() const noexcept;

private:
    [[nodiscard]] const Framebuffer* framebuffer() const noexcept;
    [[nodiscard]] const Buffer* uniformBuffer() const noexcept;
    [[nodiscard]] const Pipeline* pipeline() const noexcept;

    DirectionalLightData m_data;
    Handle<Framebuffer> m_framebuffer;
    Handle<Buffer> m_uniformBuffer;
    Handle<Pipeline> m_pipeline;
    ResourceManager* m_mngr;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
