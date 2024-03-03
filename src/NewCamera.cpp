#include "NewCamera.hpp"

#include <glm/gtx/euler_angles.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

NewCamera::NewCamera(NewCameraDescriptor desc)
    : m{std::move(desc)}
{
    glm::vec2 eulerAngles = glm::eulerAngles(m.orientation);
    m_pitch = eulerAngles.x;
    m_yaw = eulerAngles.y;

    updateView();
    updateProjection();
}

//------------------------------------------------------------------------

void NewCamera::move()
{
    
}

//------------------------------------------------------------------------

void NewCamera::rotate(float deltaX, float deltaY)
{
    static constexpr float pitchBound = glm::half_pi<float>() - 0.01f;
    
    m_pitch = glm::clamp(m_pitch + deltaY, -pitchBound, pitchBound);
    m_yaw += deltaX;

    m.orientation = glm::angleAxis(m_yaw, util::up) * glm::angleAxis(m_pitch, util::left);

    updateView();
}

//------------------------------------------------------------------------

void NewCamera::updateView()
{
    glm::vec3 target = m.orientation * util::front;
    glm::vec3 up = m.orientation * util::up;

    m_matrices.viewMatT = glm::transpose(glm::lookAt(m.position, m.position + target, up));
}

//------------------------------------------------------------------------

void NewCamera::updateProjection()
{
    float width = m.viewportDims.x;
    float height = m.viewportDims.y;

    if (m.fov) {
        m_matrices.projMat = glm::perspective(m.fov.value(), width / height, m.zNear, m.zFar);
    } else {
        m_matrices.projMat = glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, m.zNear, m.zFar);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
