#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <array>
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
        std::array<bool, 512> keys = { false };
        float pitch = 0.0f;
        float yaw = -glm::half_pi<float>();
    };

    App() = default;
    ~App();

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = default;
    App& operator=(App&&) = default;
           
    GLFWwindow* getGlCtx() const noexcept { return m_window; }
    float getDeltaTime() const noexcept { return m_deltaTime; }
    const GLFWState& getGLFWState() const noexcept { return s_state; }

    void init() const noexcept;
    void updateInternalTimes() const noexcept;

    // According to the GLFW input reference.
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    
        s_state.keys[key] = static_cast<bool>(action);
    }

    // According to the GLFW input reference.
    static void mouseCallback(GLFWwindow* window, double xPos, double yPos)
    {
        static float xPosPrev = xPos;
        static float yPosPrev = yPos;

        float xOffset = mouse_sensitivity * (xPos - xPosPrev);
        float yOffset = mouse_sensitivity * (yPosPrev - yPos);

        static constexpr float pitchBound = glm::half_pi<float>() - 0.01f;
        s_state.pitch = glm::clamp(s_state.pitch + yOffset, -pitchBound, pitchBound);
        s_state.yaw += xOffset;

        xPosPrev = xPos;
        yPosPrev = yPos;
    }

    static constexpr std::string_view name = "Zhade - ESC to quit";
    static constexpr uint32_t s_windowWidth = 800u;
    static constexpr uint32_t s_windowHeight = 800u;
    static constexpr float mouse_sensitivity = 0.002f;

private:
    inline static GLFWState s_state;

    mutable GLFWwindow* m_window;
    mutable float m_deltaTime = 0.0f;
    mutable float m_framePrev = 0.0f;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
