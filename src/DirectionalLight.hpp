#pragma once

#include "Framebuffer.hpp"

#include <glm/glm.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class ResourceManager;

struct DirectionalLightDescriptor
{
    glm::vec3 direction;
    glm::vec3 irradiance;
    glm::ivec2 shadowMapDims;
    ResourceManager* mngr;
};

//------------------------------------------------------------------------

class DirectionalLight
{
public:
    explicit DirectionalLight(DirectionalLightDescriptor desc);

    [[nodiscard]] const Framebuffer* framebuffer() const noexcept;

private:
    glm::vec3 m_direction;
    glm::vec3 m_irradiance;
    Handle<Framebuffer> m_framebuffer;
    ResourceManager* m_mngr;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
