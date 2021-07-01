#pragma once

#include "Util.hpp"
#include "App.hpp"
#include "ShaderProgram.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>

//------------------------------------------------------------------------

class Camera {
public:
    struct Specification {
        glm::vec3 position = glm::vec3(0.0f,  1.0f,  3.0f);
        glm::vec3 target   = glm::vec3(0.0f,  0.0f, -1.0f);
        glm::vec3 up       = glm::vec3(0.0f,  1.0f,  0.0f);
        float zNear        = 0.1f;
        float zFar         = 1000.0f;

        Specification() = default;
        Specification(Specification&& specification) = default;
        virtual ~Specification() = default;
    };

    virtual ~Camera() = default;

    inline void updateView()                 
    {
        m_view = glm::lookAt(m_specification->position, m_specification->position + m_specification->target, m_specification->up);
    }
    
    virtual inline void updateProjectivity() = 0;

    inline const glm::mat4& getView() const                      { return m_view; }
    inline const glm::mat4& getProjectivity() const              { return m_projectivity; }
    virtual inline const Specification* getSpecification() const = 0;

    bool move();
    bool rotate();
    void pushViewMatrix(ShaderProgram& shaderProgram);
    void pushProjectionMatrix(ShaderProgram& shaderProgram);

    static constexpr float s_cameraBaseSpeed = 2.5f;

protected:
    Camera(std::unique_ptr<Specification> specification);

    std::unique_ptr<Specification> m_specification;
    glm::mat4 m_view         = glm::mat4(1.0f);
    glm::mat4 m_projectivity = glm::mat4(1.0f);
};

//------------------------------------------------------------------------

class OrthographicCamera final : public Camera {
public:
    struct Specification final : Camera::Specification {
        glm::vec2 xRange;
        glm::vec2 yRange;

        Specification(std::unique_ptr<Camera::Specification> baseSpecification = std::make_unique<Camera::Specification>(),
                      const glm::vec2& xRange                                  = glm::vec2(-5.0f, 5.0f),
                      const glm::vec2& yRange                                  = glm::vec2(-5.0f, 5.0f))
            : Camera::Specification(std::move(*baseSpecification)),
              xRange{xRange},
              yRange{yRange}
        {
            baseSpecification.release();
        }
    };

    OrthographicCamera(std::unique_ptr<Specification> specification = std::make_unique<Specification>());

    virtual inline void updateProjectivity() override
    {
        auto specification = getSpecification();
        m_projectivity = glm::ortho(specification->xRange[0],
                                    specification->xRange[1],
                                    specification->yRange[0],
                                    specification->yRange[1],
                                    specification->zNear,
                                    specification->zFar);
    }

    virtual inline const Specification* getSpecification() const { return static_cast<Specification*>(m_specification.get()); }
};

//------------------------------------------------------------------------

class PerspectiveCamera final : public Camera {
public:
    struct Specification final : Camera::Specification {
        float fov;
        float aspect_ratio;

        Specification(std::unique_ptr<Camera::Specification> baseSpecification = std::make_unique<Camera::Specification>(),
                      float fov                                                = 70.0f,
                      float aspect_ratio                                       = (float)App::window_width / (float)App::window_height)
            : Camera::Specification(std::move(*baseSpecification)),
              fov{fov},
              aspect_ratio{aspect_ratio}
        {
            baseSpecification.release();
        }
    };

    PerspectiveCamera(std::unique_ptr<Specification> specification = std::make_unique<Specification>());

    virtual inline void updateProjectivity() override
    {
        auto specification = getSpecification();
        m_projectivity = glm::perspective(specification->fov, specification->aspect_ratio, specification->zNear, specification->zFar);
    }

    virtual inline const Specification* getSpecification() const { return static_cast<Specification*>(m_specification.get()); }
};

//------------------------------------------------------------------------
