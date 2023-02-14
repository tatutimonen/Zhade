#include "Camera.hpp"

#include "constants.hpp"
#include "util.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

Camera::Camera(ResourceManager* mngr, const App* app, const Settings& settings)
    : m_mngr{mngr},
      m_app{app},
      m_settings{std::move(settings)},
      m_matrices{Matrices()},
      m_uniformBuffer{mngr->createBuffer(GL_UNIFORM_BUFFER, static_cast<GLsizei>(sizeof(Matrices)))}
{
    m_mngr->getBuffer(m_uniformBuffer)->bindBase(constants::CAMERA_BINDING);
    updateView();
}

//------------------------------------------------------------------------

void Camera::tick() const noexcept
{
    const bool moved = move();
    const bool rotated = rotate();

    if (moved || rotated)
    {
        m_mngr->getBuffer(m_uniformBuffer)->setData<Matrices>(&m_matrices, 1, 0);
    }
}

//------------------------------------------------------------------------

bool Camera::move() const noexcept
{
    const auto& [keys, pitch, yaw] = m_app->getGLFWState();
    const float cameraSpeed = s_cameraSpeed * m_app->getDeltaTime();

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

bool Camera::rotate() const noexcept
{
    [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app->getGLFWState();

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
