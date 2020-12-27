#pragma once

#include "gl_common.hpp"
#include "App.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
    static constexpr float camera_base_speed = 0.05f;
public:
    Camera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far);
    virtual ~Camera() = default;

    void set_position(const glm::vec3& position) { m_position = position; }
    void set_target(const glm::vec3& target)     { m_target = target; }
    void set_up(const glm::vec3& up)             { m_up = up; }
    virtual void set_view() = 0;

    const glm::vec3& get_position() const     { return m_position; }
    const glm::vec3& get_target() const       { return m_target; }
    const glm::vec3& get_up() const           { return m_up; }
    float get_z_near() const                  { return m_z_near; }
    float get_z_far() const                   { return m_z_far; }
    const glm::mat4& get_view() const         { return m_view; }
    const glm::mat4& get_projectivity() const { return m_projectivity; }

    void move();
    void rotate();
    void push_view_matrix(GLint location);
    void push_projection_matrix(GLint location);

protected:
    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
    float m_z_near;
    float m_z_far;
    glm::mat4 m_view;
    glm::mat4 m_projectivity;
};

class OrthographicCamera : public Camera {
public:
    OrthographicCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                       float x0, float x1, float y0, float y1);
    ~OrthographicCamera() = default;

    void set_view(const glm::vec2& x_range, const glm::vec2& y_range, float z_near, float z_far);

    const glm::vec2& get_x_range() const { return m_x_range; }
    const glm::vec2& get_y_range() const { return m_y_range; }

private:
    glm::vec2 m_x_range;
    glm::vec2 m_y_range;
};

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up, float z_near, float z_far,
                      float fov, float aspect_ratio);
    ~PerspectiveCamera() = default;

    void set_view(float fov, float aspect_ratio, float z_near, float z_far);

    float get_fov() const          { return m_fov; }
    float get_aspect_ratio() const { return m_aspect_ratio; }

private:
    float m_fov;
    float m_aspect_ratio;
};