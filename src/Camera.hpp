#pragma once

#include "App.hpp"
#include "constants.hpp"
#include "Observer.hpp"
#include "ShaderProgram.hpp"
#include "Subject.hpp"
#include "UniformBuffer.hpp"
#include "util.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <memory>
#include <set>

//------------------------------------------------------------------------

class Camera {
public:
    struct Settings {
        glm::vec3 center = glm::vec3(0.0f,  1.0f,  3.0f);
        glm::vec3 target = glm::vec3(0.0f,  0.0f, -1.0f);
        glm::vec3 up     = glm::vec3(0.0f,  1.0f,  0.0f);
        float zNear      = 0.1f;
        float zFar       = 1000.0f;

        Settings() = default;
        Settings(Settings&& settings) = default;
        virtual ~Settings() = default;
    };
    struct Matrices {
        glm::mat4 V = glm::mat4(1.0f);
        glm::mat4 P = glm::mat4(1.0f);
    };

    virtual ~Camera() = default;

    inline const glm::mat4& getView() const            { return m_matrices.V; }
    inline const glm::mat4& getProjectivity() const    { return m_matrices.P; }
    virtual inline const Settings& getSettings() const = 0;

    inline void updateView() 
    {
        m_matrices.V = glm::lookAt(m_settings->center, m_settings->center + m_settings->target, m_settings->up);
    }
    virtual void updateProjectivity() = 0;
    void tick() noexcept;

    static constexpr float s_cameraBaseSpeed = 5.0f;

protected:
    Camera(std::unique_ptr<Settings> settings);
    bool move();
    bool rotate();

    std::unique_ptr<Settings> m_settings;
    Matrices m_matrices;
    UniformBuffer m_uniformBuffer;
};

//------------------------------------------------------------------------
