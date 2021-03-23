#include "Camera.hpp"

Camera::Camera(std::unique_ptr<Specification> spec)
    : m_spec(std::move(spec))
{
    set_view();
}

bool Camera::move()
{
    const glm::vec3 position_prev = m_spec->position;
    const auto keys = App::get_instance().get_keys();
    const float camera_speed = s_camera_base_speed * App::get_instance().get_delta_time();
    
    if (keys[GLFW_KEY_W])
        m_spec->position += camera_speed * m_spec->target;
    if (keys[GLFW_KEY_S]) 
        m_spec->position += camera_speed * -m_spec->target;
    if (keys[GLFW_KEY_D])
        m_spec->position += camera_speed * glm::normalize(glm::cross(m_spec->target, m_spec->up));
    if (keys[GLFW_KEY_A])
        m_spec->position += camera_speed * -glm::normalize(glm::cross(m_spec->target, m_spec->up));
    if (keys[GLFW_KEY_SPACE])
        m_spec->position += camera_speed * Util::make_unit_vec3y();
    if (keys[GLFW_KEY_LEFT_SHIFT])
        m_spec->position += camera_speed * -Util::make_unit_vec3y();
    
    if (m_spec->position != position_prev) {
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
    const glm::vec3 target_prev = m_spec->target;
    m_spec->target.x = glm::cos(pitch) * glm::cos(yaw);
    m_spec->target.y = glm::sin(pitch);
    m_spec->target.z = glm::cos(pitch) * glm::sin(yaw);
    m_spec->target = glm::normalize(m_spec->target);

    if (m_spec->target != target_prev) {
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


OrthographicCamera::OrthographicCamera(std::unique_ptr<Specification> spec)
    : Camera(std::move(spec))
{
    set_projectivity();
}

void OrthographicCamera::set_projectivity()
{
    auto spec = get_spec();
    m_projectivity = glm::ortho(spec->x_range[0], spec->x_range[1], spec->y_range[0], spec->y_range[1], spec->z_near, spec->z_far);
}


PerspectiveCamera::PerspectiveCamera(std::unique_ptr<Specification> spec)
    : Camera(std::move(spec))
{
    set_projectivity();
}

void PerspectiveCamera::set_projectivity()
{
    auto spec = get_spec();
    m_projectivity = glm::perspective(spec->fov, spec->aspect_ratio, spec->z_near, spec->z_far);
}
