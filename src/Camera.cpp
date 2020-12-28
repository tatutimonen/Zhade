#include "Camera.hpp"


Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far)
    : m_position(position), m_target(target), m_up(up), m_z_near(z_near), m_z_far(z_far)
{
    set_view();
}

void Camera::set_view()
{
    m_view = glm::lookAt(m_position, m_position + m_target, m_up);
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
    
    if (m_position != position_prev) {
        set_view();
        set_projectivity();
    }
}

void Camera::push_view_matrix(GLint location)
{
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_view)));
}

void Camera::push_projection_matrix(GLint location)
{
    GL_CALL(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m_projectivity)));
}


OrthographicCamera::OrthographicCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                                       float x0, float x1, float y0, float y1)
    : Camera(position, target, up, z_near, z_far),
      m_x_range(glm::vec2(x0, x1)), m_y_range(glm::vec2(y0, y1))
{
    set_projectivity();
}

void OrthographicCamera::set_projectivity()
{
    m_projectivity = glm::ortho(m_x_range[0], m_x_range[1], m_y_range[0], m_y_range[1], m_z_near, m_z_far);
}


PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                                     float fov, float aspect_ratio)
    : Camera(position, target, up, z_near, z_far),
      m_fov(fov), m_aspect_ratio(aspect_ratio)
{
    set_projectivity();
}

void PerspectiveCamera::set_projectivity()
{
    m_projectivity = glm::perspective(m_fov, m_aspect_ratio, m_z_near, m_z_far);
}
