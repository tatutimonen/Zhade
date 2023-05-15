#pragma once

#include "App.hpp"
#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "util.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
extern "C" {
#include <GLFW/glfw3.h>
}

#include <variant>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

enum class CameraType
{
    PERSPECTIVE = 0,
    ORTHOGRAPHIC = 1
};

//------------------------------------------------------------------------

template<CameraType T = CameraType::PERSPECTIVE>
class Camera final
{
public:
    struct Settings
    {
        glm::vec3 center = glm::vec3(0.0f,  1.0f,  3.0f);
        glm::vec3 target = glm::vec3(0.0f,  0.0f, -1.0f);
        glm::vec3 up     = glm::vec3(0.0f,  1.0f,  0.0f);
        float zNear      = 0.1f;
        float zFar       = 1000.0f;
    };

    struct SettingsPerspective
    {
        float fov = 70.0f;
        float aspectRatio = static_cast<float>(App::s_windowWidth) / App::s_windowHeight;
    };

    Camera(ResourceManager* mngr, App* app, const Settings& settings = Settings{}, const SettingsPerspective& specialSettings = SettingsPerspective{})
    requires (T == CameraType::PERSPECTIVE)
        : m_mngr{mngr},
          m_app{app},
          m_settings{std::move(settings)},
          m_specialSettings{std::move(specialSettings)},
          m_uniformBuffer{mngr->createBuffer(GL_UNIFORM_BUFFER, static_cast<GLsizei>(sizeof(Matrices)))}
    {
        uniformBuffer()->bindBase(constants::CAMERA_BINDING);
        updateView();
        updateProjectivity();
    }

    ~Camera()
    {
        m_mngr->destroy(m_uniformBuffer);
    }

    void update() const noexcept
    {
        const bool moved = move();
        const bool rotated = rotate();

        if (moved || rotated)
            uniformBuffer()->setData<glm::mat3x4>(&m_matrices.VT, offsetof(Matrices, VT));
    }

    // According to the GLFW input reference.
    static void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset) noexcept
    {
        s_cameraSpeed = std::max(1.0f, s_cameraSpeed + 2.0f*static_cast<float>(yoffset));
    }

private:
    struct Matrices
    {
        glm::mat3x4 VT;
        glm::mat4 P;
    };

    const Buffer* uniformBuffer() const noexcept { return m_mngr->get(m_uniformBuffer); }

    void updateView() const noexcept
    {
        m_matrices.VT = glm::mat3x4(
            glm::transpose(glm::lookAt(m_settings.center, m_settings.center + m_settings.target, m_settings.up))
        );
    }

    void updateProjectivity() const noexcept
    {
        if constexpr (T == CameraType::PERSPECTIVE)
        {
            const auto& perspectiveSettings = std::get<SettingsPerspective>(m_specialSettings);
            m_matrices.P = glm::perspective(perspectiveSettings.fov, perspectiveSettings.aspectRatio, m_settings.zNear, m_settings.zFar);
            uniformBuffer()->setData<glm::mat4>(&m_matrices.P, offsetof(Matrices, P));
        }
    }

    bool move() const noexcept
    {
        [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app->getGLFWState();
        const float cameraSpeed = s_cameraSpeed * m_app->getDeltaTime();

        const glm::vec3 centerPrev = m_settings.center;
        
        if (keys[GLFW_KEY_W])
            m_settings.center += cameraSpeed * m_settings.target;
        if (keys[GLFW_KEY_S]) 
            m_settings.center += cameraSpeed * -m_settings.target;
        if (keys[GLFW_KEY_D])
            m_settings.center += cameraSpeed * glm::normalize(glm::cross(m_settings.target, m_settings.up));
        if (keys[GLFW_KEY_A])
            m_settings.center += cameraSpeed * -glm::normalize(glm::cross(m_settings.target, m_settings.up));
        if (keys[GLFW_KEY_SPACE])
            m_settings.center += cameraSpeed * util::makeUnitVec3y();
        if (keys[GLFW_KEY_LEFT_SHIFT])
            m_settings.center += cameraSpeed * -util::makeUnitVec3y();
        
        if (m_settings.center != centerPrev)
        {
            updateView();
            return true;
        }

        return false;
    }

    bool rotate() const noexcept
    {
        [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app->getGLFWState();

        const glm::vec3 targetPrev = m_settings.target;
        
        m_settings.target.x = glm::cos(pitch) * glm::cos(yaw);
        m_settings.target.y = glm::sin(pitch);
        m_settings.target.z = glm::cos(pitch) * glm::sin(yaw);
        m_settings.target = glm::normalize(m_settings.target);

        if (m_settings.target != targetPrev)
        {
            updateView();
            return true;
        }

        return false;
    }

    static inline float s_cameraSpeed = 5.0f;

    ResourceManager* m_mngr;
    App* m_app;
    mutable Settings m_settings;
    mutable std::variant<SettingsPerspective> m_specialSettings;
    mutable Matrices m_matrices;
    Handle<Buffer> m_uniformBuffer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
