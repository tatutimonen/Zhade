#include "Camera.hpp"

#include "constants.hpp"
#include "util.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Camera::Camera(const App& app, const Settings& settings)
    : m_app{app},
      m_settings{std::move(settings)},
      m_matrices{Matrices()},
      m_uniformBuffer{Buffer<Matrices, GL_UNIFORM_BUFFER>(sizeof(Matrices))}
{
    m_uniformBuffer.bindBase(constants::CAMERA_BINDING);
    updateView();
}

//------------------------------------------------------------------------

void Camera::tick() noexcept
{
    const bool moved = move();
    const bool rotated = rotate();

    if (moved || rotated)
    {
        m_uniformBuffer.setData(&m_matrices, 1, 0);
    }
}

//------------------------------------------------------------------------

bool Camera::move()
{
    const auto& [keys, pitch, yaw] = m_app.getGLFWState();
    const float cameraSpeed = s_cameraBaseSpeed * m_app.getDeltaTime();

    const glm::vec3 centerPrev = m_settings.center;
    
    if (keys[GLFW_KEY_W])
        m_settings.center += cameraSpeed * m_settings.target;
    if (keys[GLFW_KEY_S]) 
        m_settings.center += cameraSpeed * -m_settings.target;
    if (keys[GLFW_KEY_D])
        m_settings.center += cameraSpeed * glm::normalize(glm::cross(m_settings.target, m_settings.up));
    if (keys[GLFW_KEY_A])
        m_settings.center += cameraSpeed * -glm::normalize(glm::cross(m_settings.target, m_settings.up));
    if (keys[GLFW_KEY_SPACE])
        m_settings.center += cameraSpeed * util::makeUnitVec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_settings.center += cameraSpeed * -util::makeUnitVec3y();
    
    if (m_settings.center != centerPrev)
    {
        updateView();
        updateProjectivity();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------

bool Camera::rotate()
{
    [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app.getGLFWState();

    const glm::vec3 targetPrev = m_settings.target;
    
    m_settings.target.x = glm::cos(pitch) * glm::cos(yaw);
    m_settings.target.y = glm::sin(pitch);
    m_settings.target.z = glm::cos(pitch) * glm::sin(yaw);
    m_settings.target = glm::normalize(m_settings.target);

    if (m_settings.target != targetPrev)
    {
        updateView();
        updateProjectivity();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
