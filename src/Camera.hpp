#pragma once

#include "Handle.hpp"
#include "ResourceManager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
extern "C" {
#include <GLFW/glfw3.h>
}

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class App;

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

    virtual ~Camera()
    {
        m_mngr->destroy(m_uniformBuffer);
    }

    const glm::mat3x4& getView() const noexcept { return m_matrices.VT; }
    const glm::mat4& getProjectivity() const noexcept { return m_matrices.P; }

    void updateView() const noexcept
    {
        m_matrices.VT = glm::mat3x4(
            glm::transpose(glm::lookAt(m_settings.center, m_settings.center + m_settings.target, m_settings.up))
        );
    }

    void update() const noexcept;

    virtual const Settings& getSettings() const noexcept = 0;
    virtual void updateProjectivity() const noexcept = 0;

    // According to the GLFW input reference.
    static void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) noexcept
    {
        s_cameraSpeed = std::max(1.0f, s_cameraSpeed + 2.0f*static_cast<float>(yoffset));
    }

    static inline float s_cameraSpeed = 5.0f;

protected:
    Camera(ResourceManager* mngr, App* app, const Settings& settings);

    const Buffer* uniformBuffer() const noexcept { return m_mngr->get(m_uniformBuffer); }

    bool move() const noexcept;
    bool rotate() const noexcept;

    ResourceManager* m_mngr;
    App* m_app;
    mutable Settings m_settings;
    mutable Matrices m_matrices;
    Zhade::Handle<Buffer> m_uniformBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
