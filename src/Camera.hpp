#pragma once

#include "App.hpp"
#include "Handle.hpp"
#include "ResourceManager.hpp"
#include "common.hpp"
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
    PERSPECTIVE,
    ORTHO
};

struct CameraSettings
{
    glm::vec3 center{-388.0f, 592.0f, 154.0f};
    glm::vec3 target{0.869f,  0.341f, -0.357f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float zNear = 5.0f;
    float zFar = 5000.0f;
};

struct PerspectiveSettings
{
    float fov = 70.0f;
    float aspectRatio = implicit_cast<float>(App::s_windowWidth) / App::s_windowHeight;
};

struct OrthoSettings
{
    float xmin{}, xmax{App::s_windowWidth}, ymin{}, ymax{App::s_windowHeight};
};

using VarCameraSettings = std::variant<PerspectiveSettings, OrthoSettings>;

struct CameraDescriptor
{
    CameraSettings settings{};
    VarCameraSettings varSettings = PerspectiveSettings{};
    Handle<Buffer> uniformBuffer{};
    ResourceManager* mngr;
    App* app;
};

//------------------------------------------------------------------------

template<CameraType T = CameraType::PERSPECTIVE>
class Camera
{
public:
    explicit Camera(CameraDescriptor desc)
        : m_settings{desc.settings},
          m_varSettings{desc.varSettings},
          m_uniformBuffer{desc.uniformBuffer},
          m_mngr{desc.mngr},
          m_app{desc.app}
    {
        updateView();
        updateProjectivity();
    }
    Camera() = default;

    [[nodiscard]] const glm::vec3& center() { return m_settings.center; }
    [[nodiscard]] const glm::vec3& target() { return m_settings.target; }
    [[nodiscard]] const glm::vec3& up() { return m_settings.up; }

    void update()
    {
        const bool moved = move();
        const bool rotated = rotate();

        if (moved or rotated) {
            uniformBuffer()->setData<glm::mat3x4>(&m_matrices.viewMatT, offsetof(ViewProjMatrices, viewMatT));
        }
    }

    // According to the GLFW input reference.
    static void scrollCallback([[maybe_unused]] GLFWwindow* window, [[maybe_unused]] double xoffset, double yoffset)
    {
        s_cameraSpeed = std::max(1.0f, s_cameraSpeed + 2.0f * implicit_cast<float>(yoffset));
    }

private:
    Buffer* uniformBuffer() { return m_mngr->get(m_uniformBuffer); }

    void updateView()
    {
        m_matrices.viewMatT = glm::transpose(glm::lookAt(m_settings.center, m_settings.center + m_settings.target, m_settings.up));
    }

    void updateProjectivity()
    {
        if constexpr (T == CameraType::PERSPECTIVE) {
            const auto [fov, aspectRatio] = std::get<PerspectiveSettings>(m_varSettings);
            m_matrices.projMat = glm::perspective(fov, aspectRatio, m_settings.zNear, m_settings.zFar);
        }
        else if constexpr (T == CameraType::ORTHO) {
            const auto [xmin, xmax, ymin, ymax] = std::get<OrthoSettings>(m_varSettings);
            m_matrices.projMat = glm::ortho(xmin, xmax, ymin, ymax, m_settings.zNear, m_settings.zFar);
        }
        uniformBuffer()->setData<glm::mat4>(&m_matrices.projMat, offsetof(ViewProjMatrices, projMat));
    }

    bool move()
    {
        [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app->getGLFWState();
        const float cameraSpeed = s_cameraSpeed * m_app->deltaTime();

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

        if (m_settings.center != centerPrev) {
            updateView();
            return true;
        }

        return false;
    }

    bool rotate()
    {
        [[maybe_unused]] const auto& [keys, pitch, yaw] = m_app->getGLFWState();

        const glm::vec3 tarprev = m_settings.target;
        
        m_settings.target.x = glm::cos(pitch) * glm::cos(yaw);
        m_settings.target.y = glm::sin(pitch);
        m_settings.target.z = glm::cos(pitch) * glm::sin(yaw);
        m_settings.target = glm::normalize(m_settings.target);

        if (m_settings.target != tarprev) {
            updateView();
            return true;
        }

        return false;
    }

    static inline float s_cameraSpeed = 5.0f;

    CameraSettings m_settings;
    VarCameraSettings m_varSettings;
    ViewProjMatrices m_matrices;
    Handle<Buffer> m_uniformBuffer;
    ResourceManager* m_mngr;
    App* m_app;

    friend class Renderer;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
