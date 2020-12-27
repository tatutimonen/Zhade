#include "Camera.hpp"


Camera::Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far)
    : m_position(position), m_target(target), m_up(up), m_z_near(z_near), m_z_far(z_far)
{
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