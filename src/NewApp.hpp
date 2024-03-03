#pragma once

#include "NewCamera.hpp"
#include "common.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

struct Config
{
    std::string_view windowTitle = "Zhade - ESC to quit";
    glm::u16vec2 windowSize{1920, 1080};
    glm::u8vec3 clearColor{15, 46, 101};
    float mouseSensitivity = 0.002f;
    NewCameraDescriptor cameraDesc;
};

//------------------------------------------------------------------------

class NewApp
{
public:
    explicit NewApp(Config cfg);

    void run();

    static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void glfwMouseCallback(GLFWwindow* window, double xPos, double yPos);
    static void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
        const char* message, const void* userParam);

private:
    void initGLFW(const Config& cfg);
    void initGLEW();
    void initGL(const Config& cfg);
    void initImGui();
    void initMisc();

    GLFWwindow* m_window;
    Keys m_keys;
    NewCamera m_camera;
    float m_mouseSensitivity;
};

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
