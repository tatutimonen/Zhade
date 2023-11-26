#pragma once

#include <glm/glm.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct DirectionalLightDescriptor
{
    glm::vec3 direction;
    glm::vec3 irradiance;
};

//------------------------------------------------------------------------

class DirectionalLight
{
public:
    explicit DirectionalLight(DirectionalLightDescriptor desc);

private:
    glm::vec3 m_direction;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
