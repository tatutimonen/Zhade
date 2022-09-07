#pragma once

#include "App.hpp"
#include "Buffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class Camera
{
public:
    struct Settings
    {
        glm::vec3 center = glm::vec3(0.0f,  1.0f,  3.0f);
        glm::vec3 target = glm::vec3(0.0f,  0.0f, -1.0f);
        glm::vec3 up     = glm::vec3(0.0f,  1.0f,  0.0f);
        float zNear      = 0.1f;
        float zFar       = 1000.0f;

        virtual ~Settings() = default;
    };

    struct Matrices
    {
        glm::mat3x4 VT = glm::mat3x4(1.0f);
        glm::mat4 P = glm::mat4(1.0f);
    };

    virtual ~Camera() = default;

    const glm::mat3x4& getView() const noexcept { return m_matrices.VT; }
    const glm::mat4& getProjectivity() const noexcept { return m_matrices.P; }
    virtual const Settings& getSettings() const = 0;

    void updateView()
    {
        m_matrices.VT = glm::mat3x4(
            glm::transpose(glm::lookAt(m_settings.center, m_settings.center + m_settings.target, m_settings.up))
        );
    }

    virtual void updateProjectivity() = 0;

    void tick() noexcept;

    static constexpr auto s_cameraBaseSpeed = 5.0f;

protected:
    Camera(const App& app, const Settings& settings);
    bool move();
    bool rotate();

    const App& m_app;
    Settings m_settings;
    Matrices m_matrices;
    Buffer<Matrices, GL_UNIFORM_BUFFER> m_uniformBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
