#pragma once

#include "App.hpp"
#include "ShaderProgram.hpp"
#include "Util.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <memory>
#include <set>

//------------------------------------------------------------------------

class Camera {
public:
    struct Settings {
        glm::vec3 center      = glm::vec3(0.0f,  1.0f,  3.0f);
        glm::vec3 target      = glm::vec3(0.0f,  0.0f, -1.0f);
        glm::vec3 up          = glm::vec3(0.0f,  1.0f,  0.0f);
        float zNear           = 0.1f;
        float zFar            = 1000.0f;

        Settings() = default;
        Settings(Settings&& settings) = default;
        virtual ~Settings() = default;
    };

    virtual ~Camera() = default;

    inline const glm::mat4& getView() const            { return m_view; }
    inline const glm::mat4& getProjectivity() const    { return m_projectivity; }
    virtual inline const Settings& getSettings() const = 0;

    inline void updateView() 
    {
        m_view = glm::lookAt(m_settings->center, m_settings->center + m_settings->target, m_settings->up);
    }
    virtual void updateProjectivity() = 0;
    void tick(ShaderProgram& shaderProgram) noexcept;

    static constexpr float s_cameraBaseSpeed = 2.5f;

protected:
    Camera(std::unique_ptr<Settings> settings);
    bool move();
    bool rotate();

    std::unique_ptr<Settings> m_settings;
    glm::mat4 m_view;
    glm::mat4 m_projectivity;
};

//------------------------------------------------------------------------
