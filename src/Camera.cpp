#include "Camera.hpp"


Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far)
    : m_position(position), m_target(target), m_up(up), m_z_near(z_near), m_z_far(z_far)
{
}

void Camera::move()
{
    glm::vec3 position_prev = m_position;
    auto keys = App::get_instance()->get_keys();
    if (keys[GLFW_KEY_W])
        m_position += camera_base_speed * m_target;
    if (keys[GLFW_KEY_S]) 
        m_position += camera_base_speed * -m_target;
    if (keys[GLFW_KEY_D])
        m_position += camera_base_speed * glm::normalize(glm::cross(m_target, m_up));
    if (keys[GLFW_KEY_A])
        m_position += camera_base_speed * -glm::normalize(glm::cross(m_target, m_up));
    
    if (m_position != position_prev) 
        m_view = glm::lookAt(m_position, m_position + m_target, m_up);
}

void Camera::push_view_matrix(GLint location)
{
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_view)));
}

OrthographicCamera::OrthographicCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                                       float x0, float x1, float y0, float y1)
    : Camera(position, target, up, z_near, z_far), m_x_range(glm::vec2(x0, x1)), m_y_range(glm::vec2(y0, y1))
{
}

void OrthographicCamera::set_view(const glm::vec2& x_range, const glm::vec2& y_range, float z_near, float z_far)
{
    m_view = glm::ortho(x_range[0], x_range[1], y_range[0], y_range[1], z_near, z_far);
}

PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                                     float fov, float aspect_ratio)
    : Camera(position, target, up, z_near, z_far), m_fov(fov), m_aspect_ratio(aspect_ratio)
{
}

void PerspectiveCamera::set_view(float fov, float aspect_ratio, float z_near, float z_far)
{
    m_view = glm::perspective(fov, aspect_ratio, z_near, z_far);
}