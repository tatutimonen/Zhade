#pragma once

#include "common.hpp"

#include <glm/gtc/quaternion.hpp>

#include <optional>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct NewCameraDescriptor
{
    glm::vec3 position{0.0f};
    glm::quat orientation = glm::identity<glm::quat>();
    glm::vec2 viewportDims;
    float zNear = 10.0f;
    float zFar = 10'000.0f;
    std::optional<float> fov = 70.0f;
};

//------------------------------------------------------------------------

class NewCamera
{
public:
    explicit NewCamera(NewCameraDescriptor desc);

private:
    void move();
    void rotate(float deltaX, float deltaY);
    void updateView();
    void updateProjection();

    NewCameraDescriptor m;
    ViewProjMatrices m_matrices;
    float m_pitch;
    float m_yaw;

    friend class NewApp;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
