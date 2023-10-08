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
    PERSPECTIVE,
    ORTHO
};

struct CameraSettings
{
    glm::vec3 center{0.0f, 1.0f, 3.0f};
    glm::vec3 target{0.0f, 0.0f, -1.0f};
    glm::vec3 up{0.0f, 1.0f, 0.0f};
    float zNear = 0.1f;
    float zFar = 1000.0f;
};

struct PerspectiveSettings
{
    float fov = 70.0f;
    float aspectRatio = static_cast<float>(App::s_windowWidth) / App::s_windowHeight;
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
          m_uniformBuffer{desc.mngr->createBuffer({.byteSize = static_cast<GLsizei>(sizeof(Matrices)), .usage = BufferUsage::UNIFORM})},
          m_mngr{desc.mngr},
          m_app{desc.app}
    {
        uniformBuffer()->bindBase(CAMERA_BINDING);
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

        if (moved or rotated)
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
        m_matrices.VT = glm::transpose(glm::lookAt(m_settings.center, m_settings.center + m_settings.target, m_settings.up));
    }

    void updateProjectivity() const noexcept
    {
        if constexpr (T == CameraType::PERSPECTIVE)
        {
            const auto [fov, aspectRatio] = std::get<PerspectiveSettings>(m_varSettings);
            m_matrices.P = glm::perspective(fov, aspectRatio, m_settings.zNear, m_settings.zFar);
        }
        else if (T == CameraType::ORTHO)
        {
            const auto [xmin, xmax, ymin, ymax] = std::get<OrthoSettings>(m_varSettings);
            m_matrices.P = glm::ortho(xmin, xmax, ymin, ymax, m_settings.zNear, m_settings.zFar);
        }
        uniformBuffer()->setData<glm::mat4>(&m_matrices.P, offsetof(Matrices, P));
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

    mutable CameraSettings m_settings;
    mutable VarCameraSettings m_varSettings;
    mutable Matrices m_matrices;
    Handle<Buffer> m_uniformBuffer;
    ResourceManager* m_mngr;
    App* m_app;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
