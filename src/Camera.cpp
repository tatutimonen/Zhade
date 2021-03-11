#include "Camera.hpp"

Camera::Camera(const glm::vec3& position,
               const glm::vec3& target,
               const glm::vec3& up,
               float z_near,
               float z_far)
    : m_position(position), m_target(target), m_up(up), m_z_near(z_near), m_z_far(z_far)
{
    set_view();
}

bool Camera::move()
{
    const glm::vec3 position_prev = m_position;
    const auto keys = App::get_instance().get_keys();
    const float camera_speed = s_camera_base_speed * App::get_instance().get_delta_time();
    
    if (keys[GLFW_KEY_W])
        m_position += camera_speed * m_target;
    if (keys[GLFW_KEY_S]) 
        m_position += camera_speed * -m_target;
    if (keys[GLFW_KEY_D])
        m_position += camera_speed * glm::normalize(glm::cross(m_target, m_up));
    if (keys[GLFW_KEY_A])
        m_position += camera_speed * -glm::normalize(glm::cross(m_target, m_up));
    if (keys[GLFW_KEY_SPACE])
        m_position += camera_speed * Util::make_unit_vec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_position += camera_speed * -Util::make_unit_vec3y();
    
    if (m_position != position_prev) {
        set_view();
        set_projectivity();
        return true;
    }

    return false;
}

bool Camera::rotate()
{
    const float pitch = App::get_instance().get_pitch();
    const float yaw = App::get_instance().get_yaw();
    const glm::vec3 target_prev = m_target;
    m_target.x = glm::cos(pitch) * glm::cos(yaw);
    m_target.y = glm::sin(pitch);
    m_target.z = glm::cos(pitch) * glm::sin(yaw);
    m_target = glm::normalize(m_target);

    if (m_target != target_prev) {
        set_view();
        set_projectivity();
        return true;
    }

    return false;
}

void Camera::push_view_matrix(ShaderProgram& shader_program)
{
    shader_program.set_uniform<glm::mat4>("view", glm::value_ptr(m_view));
}

void Camera::push_projection_matrix(ShaderProgram& shader_program)
{
    shader_program.set_uniform<glm::mat4>("projection", glm::value_ptr(m_projectivity));
}


OrthographicCamera::OrthographicCamera(const glm::vec3& position,
                                       const glm::vec3& target,
                                       const glm::vec3& up,
                                       float z_near,
                                       float z_far,
                                       float x0,
                                       float x1,
                                       float y0,
                                       float y1)
    : Camera(position, target, up, z_near, z_far),
      m_x_range(glm::vec2(x0, x1)), m_y_range(glm::vec2(y0, y1))
{
    set_projectivity();
}

void OrthographicCamera::set_projectivity()
{
    m_projectivity = glm::ortho(m_x_range[0], m_x_range[1], m_y_range[0], m_y_range[1], m_z_near, m_z_far);
}


PerspectiveCamera::PerspectiveCamera(const glm::vec3& position,
                                     const glm::vec3& target,
                                     const glm::vec3& up,
                                     float z_near,
                                     float z_far,
                                     float fov,
                                     float aspect_ratio)
    : Camera(position, target, up, z_near, z_far),
      m_fov(fov), m_aspect_ratio(aspect_ratio)
{
    set_projectivity();
}

void PerspectiveCamera::set_projectivity()
{
    m_projectivity = glm::perspective(m_fov, m_aspect_ratio, m_z_near, m_z_far);
}
