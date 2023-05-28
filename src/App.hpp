#pragma once

#include "constants.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
extern "C" {
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}

#include <array>
#include <cstdint>
#include <format>
#include <iostream>
#include <string_view>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

class App
{
public:
    struct GLFWState
    {
        std::array<bool, 512> keys{false};
        float pitch{};
        float yaw{-glm::half_pi<float>{}};
    };

    struct TemporalState
    {
        float deltaTime{};
        float prevTime{};
        std::array<float, constants::TEMPORAL_CACHE_SIZE> FPS;
        std::array<float, constants::TEMPORAL_CACHE_SIZE> frameTimes;
        uint8_t writeIdx{};
    };

    App() = default;
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = default;
    App& operator=(App&&) = default;
           
    GLFWwindow* getGLCtx() const noexcept { return m_window; }
    float getDeltaTime() const noexcept { return m_temporalState.deltaTime; }
    const GLFWState& getGLFWState() const noexcept { return s_state; }

    void init() const noexcept;
    void updateTemporalState() const noexcept;

    // According to the GLFW input reference.
    static void keyCallback(GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action, int mode) noexcept
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) [[unlikely]]
            glfwSetWindowShouldClose(window, GL_TRUE);
        s_state.keys[key] = static_cast<bool>(action);
    }

    // According to the GLFW input reference.
    static void mouseCallback([[maybe_unused]] GLFWwindow* window, double xPos, double yPos) noexcept
    {
        static float xPosPrev = xPos;
        static float yPosPrev = yPos;

        static constexpr float mouseSensitivity = 0.002f;
        float xOffset = mouseSensitivity * (xPos - xPosPrev);
        float yOffset = mouseSensitivity * (yPosPrev - yPos);

        static constexpr float pitchBound = glm::half_pi<float>() - 0.01f;
        s_state.pitch = glm::clamp(s_state.pitch + yOffset, -pitchBound, pitchBound);
        s_state.yaw += xOffset;

        xPosPrev = xPos;
        yPosPrev = yPos;
    }

    static void debugCallback([[maybe_unused]] GLenum source, [[maybe_unused]] GLenum type, [[maybe_unused]] GLuint id,
        GLenum severity, [[maybe_unused]] GLsizei length, const char* message, [[maybe_unused]] const void* userParam) noexcept
    {
        if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH) [[unlikely]]
            std::cerr << message << "\n";
    }

    static constexpr std::string_view s_title = "Zhade - ESC to quit";
    static constexpr uint32_t s_windowWidth = 1980u;
    static constexpr uint32_t s_windowHeight = 1080u;

private:
    static inline GLFWState s_state;

    mutable GLFWwindow* m_window;
    mutable TemporalState m_temporalState;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
