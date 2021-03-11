#pragma once

#include "Util.hpp"
#include "App.hpp"
#include "ShaderProgram.hpp"

extern "C" {
#include <GLFW/glfw3.h>
}
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:
    Camera();
    virtual ~Camera() = default;

    inline void set_position(const glm::vec3& position) { m_position = position; }
    inline void set_target(const glm::vec3& target)     { m_target = target; }
    inline void set_up(const glm::vec3& up)             { m_up = up; }
    inline void set_view()                              { m_view = glm::lookAt(m_position, m_position + m_target, m_up); }
    virtual void set_projectivity() = 0;

    inline const glm::vec3& get_position() const     { return m_position; }
    inline const glm::vec3& get_target() const       { return m_target; }
    inline const glm::vec3& get_up() const           { return m_up; }
    inline float get_z_near() const                  { return m_z_near; }
    inline float get_z_far() const                   { return m_z_far; }
    inline const glm::mat4& get_view() const         { return m_view; }
    inline const glm::mat4& get_projectivity() const { return m_projectivity; }

    bool move();
    bool rotate();
    void push_view_matrix(ShaderProgram& shader_program);
    void push_projection_matrix(ShaderProgram& shader_program);

    static constexpr float s_camera_base_speed = 2.5f;

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
                       float z_near              =  0.1f,
                       float z_far               =  100.0f,
                       float x0                  = -5.0f,
                       float x1                  =  5.0f,
                       float y0                  = -5.0f,
                       float y1                  =  5.0f);
    ~OrthographicCamera() = default;

    void set_projectivity() override;

    inline const glm::vec2& get_x_range() const { return m_x_range; }
    inline const glm::vec2& get_y_range() const { return m_y_range; }

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
                      float aspect_ratio        = (float)App::window_width / (float)App::window_height);
    ~PerspectiveCamera() = default;

    void set_projectivity() override;

    inline float get_fov() const          { return m_fov; }
    inline float get_aspect_ratio() const { return m_aspect_ratio; }

private:
    float m_fov;
    float m_aspect_ratio;
};
