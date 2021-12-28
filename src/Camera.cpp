#include "Camera.hpp"

//------------------------------------------------------------------------

Camera::Camera(std::unique_ptr<Settings> settings)
    : m_settings{std::move(settings)}
{
    updateView();
}

//------------------------------------------------------------------------

void Camera::tick(ShaderProgram& shaderProgram) noexcept
{
    const bool moved = move();
    const bool rotated = rotate();
    if (moved || rotated)
    {
        glm::mat4 VPMatrix = m_projectivity * m_view;
        shaderProgram.setUniform<glm::mat4>("u_VP", glm::value_ptr(VPMatrix));
        shaderProgram.setUniform<glm::mat4>("u_P", glm::value_ptr(m_projectivity));
        notify(m_settings->center);
    }
}

//------------------------------------------------------------------------

bool Camera::move()
{
    const glm::vec3 centerPrev = m_settings->center;
    const auto keys = App::get_instance().get_keys();
    const float camera_speed = s_cameraBaseSpeed * App::get_instance().get_delta_time();
    
    if (keys[GLFW_KEY_W])
        m_settings->center += camera_speed * m_settings->target;
    if (keys[GLFW_KEY_S]) 
        m_settings->center += camera_speed * -m_settings->target;
    if (keys[GLFW_KEY_D])
        m_settings->center += camera_speed * glm::normalize(glm::cross(m_settings->target, m_settings->up));
    if (keys[GLFW_KEY_A])
        m_settings->center += camera_speed * -glm::normalize(glm::cross(m_settings->target, m_settings->up));
    if (keys[GLFW_KEY_SPACE])
        m_settings->center += camera_speed * Util::makeUnitVec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_settings->center += camera_speed * -Util::makeUnitVec3y();
    
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
    const float pitch = App::get_instance().get_pitch();
    const float yaw = App::get_instance().get_yaw();
    const glm::vec3 targetPrev = m_settings->target;
    
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
