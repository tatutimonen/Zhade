#include "Camera.hpp"

//------------------------------------------------------------------------

Camera::Camera(std::weak_ptr<const App> app, std::unique_ptr<Settings> settings)
    : m_app{app},
      m_settings{std::move(settings)},
      m_matrices{Matrices()},
      m_uniformBuffer{UniformBuffer("Camera", constants::CAMERA_BINDING, sizeof(Matrices))}
{
    updateView();
}

//------------------------------------------------------------------------

void Camera::tick() noexcept
{
    const auto moved = move();
    const auto rotated = rotate();

    if (moved || rotated)
    {
        m_uniformBuffer.update(0, &m_matrices, sizeof(Matrices));
    }
}

//------------------------------------------------------------------------

bool Camera::move()
{
    const auto app = m_app.lock();
    const auto& [keys, pitch, yaw] = app->getGLFWState();
    const auto cameraSpeed = s_cameraBaseSpeed * app->getDeltaTime();

    const auto centerPrev = m_settings->center;
    
    if (keys[GLFW_KEY_W])
        m_settings->center += cameraSpeed * m_settings->target;
    if (keys[GLFW_KEY_S]) 
        m_settings->center += cameraSpeed * -m_settings->target;
    if (keys[GLFW_KEY_D])
        m_settings->center += cameraSpeed * glm::normalize(glm::cross(m_settings->target, m_settings->up));
    if (keys[GLFW_KEY_A])
        m_settings->center += cameraSpeed * -glm::normalize(glm::cross(m_settings->target, m_settings->up));
    if (keys[GLFW_KEY_SPACE])
        m_settings->center += cameraSpeed * util::makeUnitVec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_settings->center += cameraSpeed * -util::makeUnitVec3y();
    
    if (m_settings->center != centerPrev)
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
    const auto app = m_app.lock();
    [[maybe_unused]] const auto& [keys, pitch, yaw] = app->getGLFWState();

    const auto targetPrev = m_settings->target;
    
    m_settings->target.x = glm::cos(pitch) * glm::cos(yaw);
    m_settings->target.y = glm::sin(pitch);
    m_settings->target.z = glm::cos(pitch) * glm::sin(yaw);
    m_settings->target = glm::normalize(m_settings->target);

    if (m_settings->target != targetPrev)
    {
        updateView();
        updateProjectivity();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------
