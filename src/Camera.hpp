#pragma once

#include "Util.hpp"
#include "App.hpp"
#include "ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>


class Camera {
public:
    struct Specification {
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up;
        float z_near;
        float z_far;

        Specification(const glm::vec3& position = glm::vec3(),
                      const glm::vec3& target   = glm::vec3(0.0f, 0.0f, -1.0f),
                      const glm::vec3& up       = glm::vec3(0.0f, 1.0f, 0.0f),
                      float z_near              = 0.1f,
                      float z_far               = 100.0f)
            : position(position), target(target), up(up), z_near(z_near), z_far(z_far) {}
        virtual ~Specification() = default;
    };

    virtual ~Camera() = default;

    inline void set_view() { m_view = glm::lookAt(m_spec->position,
                                                  m_spec->position + m_spec->target,
                                                  m_spec->up); }
    virtual void set_projectivity() = 0;

    inline const glm::mat4& get_view() const         { return m_view; }
    inline const glm::mat4& get_projectivity() const { return m_projectivity; }
    virtual const Specification* get_spec() const    = 0;

    bool move();
    bool rotate();
    void push_view_matrix(ShaderProgram& shader_program);
    void push_projection_matrix(ShaderProgram& shader_program);

    static constexpr float s_camera_base_speed = 2.5f;

protected:
    Camera(std::unique_ptr<Specification> base_spec);

    std::unique_ptr<Specification> m_spec;
    glm::mat4 m_view         = glm::mat4(1.0f);
    glm::mat4 m_projectivity = glm::mat4(1.0f);
};


class OrthographicCamera final : public Camera {
public:
    struct Specification : Camera::Specification {
        glm::vec2 x_range;
        glm::vec2 y_range;

        Specification(std::unique_ptr<Camera::Specification> base_spec = std::make_unique<Camera::Specification>(),
                      const glm::vec2& x_range                         = glm::vec2(-5.f, 5.f),
                      const glm::vec2& y_range                         = glm::vec2(-5.f, 5.f))
            : Camera::Specification(*base_spec.get()),
              x_range(x_range), y_range(y_range) {}
    };

    OrthographicCamera(std::unique_ptr<Specification> spec = std::make_unique<Specification>());
    ~OrthographicCamera() = default;

    virtual void set_projectivity() override;

    virtual const Specification* get_spec() const { return dynamic_cast<Specification*>(m_spec.get()); }
};


class PerspectiveCamera final : public Camera {
public:
    struct Specification : Camera::Specification {
        float fov;
        float aspect_ratio;

        Specification(std::unique_ptr<Camera::Specification> base_spec = std::make_unique<Camera::Specification>(),
                      float fov                                        = 70.f,
                      float aspect_ratio                               = (float)App::window_width / (float)App::window_height)
            : Camera::Specification(*base_spec.get()),
              fov(fov), aspect_ratio(aspect_ratio) {}
    };

    PerspectiveCamera(std::unique_ptr<Specification> spec = std::make_unique<Specification>());
    ~PerspectiveCamera() = default;

    virtual void set_projectivity() override;

    virtual const Specification* get_spec() const { return dynamic_cast<Specification*>(m_spec.get()); }
};
