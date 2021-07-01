#include "Camera.hpp"

//------------------------------------------------------------------------

Camera::Camera(std::unique_ptr<Specification> specification)
    : m_specification{std::move(specification)}
{
    updateView();
}

//------------------------------------------------------------------------

bool Camera::move()
{
    const glm::vec3 position_prev = m_specification->position;
    const auto keys = App::get_instance().get_keys();
    const float camera_speed = s_cameraBaseSpeed * App::get_instance().get_delta_time();
    
    if (keys[GLFW_KEY_W])
        m_specification->position += camera_speed * m_specification->target;
    if (keys[GLFW_KEY_S]) 
        m_specification->position += camera_speed * -m_specification->target;
    if (keys[GLFW_KEY_D])
        m_specification->position += camera_speed * glm::normalize(glm::cross(m_specification->target, m_specification->up));
    if (keys[GLFW_KEY_A])
        m_specification->position += camera_speed * -glm::normalize(glm::cross(m_specification->target, m_specification->up));
    if (keys[GLFW_KEY_SPACE])
        m_specification->position += camera_speed * Util::makeUnitVec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_specification->position += camera_speed * -Util::makeUnitVec3y();
    
    if (m_specification->position != position_prev) {
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
    const glm::vec3 target_prev = m_specification->target;
    
    m_specification->target.x = glm::cos(pitch) * glm::cos(yaw);
    m_specification->target.y = glm::sin(pitch);
    m_specification->target.z = glm::cos(pitch) * glm::sin(yaw);
    m_specification->target = glm::normalize(m_specification->target);

    if (m_specification->target != target_prev) {
        updateView();
        updateProjectivity();
        return true;
    }

    return false;
}

//------------------------------------------------------------------------

void Camera::pushViewMatrix(ShaderProgram& shaderProgram)
{
    shaderProgram.setUniform<glm::mat4>("u_view", glm::value_ptr(m_view));
}

//------------------------------------------------------------------------

void Camera::pushProjectionMatrix(ShaderProgram& shaderProgram)
{
    shaderProgram.setUniform<glm::mat4>("u_projection", glm::value_ptr(m_projectivity));
}

//------------------------------------------------------------------------

OrthographicCamera::OrthographicCamera(std::unique_ptr<Specification> specification)
    : Camera{std::move(specification)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------

PerspectiveCamera::PerspectiveCamera(std::unique_ptr<Specification> specification)
    : Camera{std::move(specification)}
{
    updateProjectivity();
}

//------------------------------------------------------------------------
