#pragma once

#include "gl_common.hpp"
#include "App.hpp"
#include "ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
    static constexpr float s_camera_base_speed = 2.5f;
public:
    Camera();
    virtual ~Camera() = default;

    void set_position(const glm::vec3& position) { m_position = position; }
    void set_target(const glm::vec3& target)     { m_target = target; }
    void set_up(const glm::vec3& up)             { m_up = up; }
    void set_view();
    virtual void set_projectivity() = 0;

    const glm::vec3& get_position() const     { return m_position; }
    const glm::vec3& get_target() const       { return m_target; }
    const glm::vec3& get_up() const           { return m_up; }
    float get_z_near() const                  { return m_z_near; }
    float get_z_far() const                   { return m_z_far; }
    const glm::mat4& get_view() const         { return m_view; }
    const glm::mat4& get_projectivity() const { return m_projectivity; }

    bool move();
    bool rotate();
    void push_view_matrix(std::shared_ptr<ShaderProgram> program);
    void push_projection_matrix(std::shared_ptr<ShaderProgram> program);

protected:
    Camera(const glm::vec3& position,
           const glm::vec3& target,
           const glm::vec3& up,
           float z_near,
           float z_far);

    glm::vec3 m_position;
    glm::vec3 m_target;
    glm::vec3 m_up;
    float m_z_near;
    float m_z_far;
    glm::mat4 m_view = glm::mat4(1.0f);
    glm::mat4 m_projectivity = glm::mat4(1.0f);
};


class OrthographicCamera final : public Camera {
public:
    OrthographicCamera(const glm::vec3& position = glm::vec3(),
                       const glm::vec3& target   = glm::vec3(0.0f, 0.0f, -1.0f),
                       const glm::vec3& up       = glm::vec3(0.0f, 1.0f, 0.0f),
                       float z_near              = 0.1f,
                       float z_far               = 100.0f,
                       float x0                  = -5.0f,
                       float x1                  = 5.0f,
                       float y0                  = -5.0f,
                       float y1                  = 5.0f);
    ~OrthographicCamera() = default;

    void set_projectivity();

    const glm::vec2& get_x_range() const { return m_x_range; }
    const glm::vec2& get_y_range() const { return m_y_range; }

private:
    glm::vec2 m_x_range;
    glm::vec2 m_y_range;
};


class PerspectiveCamera final : public Camera {
public:
    PerspectiveCamera(const glm::vec3& position = glm::vec3(),
                      const glm::vec3& target   = glm::vec3(0.0f, 0.0f, -1.0f),
                      const glm::vec3& up       = glm::vec3(0.0f, 1.0f, 0.0f),
                      float z_near              = 0.1f,
                      float z_far               = 100.0f,
                      float fov                 = 70.0f,
                      float aspect_ratio        = static_cast<float>(App::window_width) \
                                                  / static_cast<float>(App::window_height));
    ~PerspectiveCamera() = default;

    void set_projectivity();

    float get_fov() const          { return m_fov; }
    float get_aspect_ratio() const { return m_aspect_ratio; }

private:
    float m_fov;
    float m_aspect_ratio;
};
