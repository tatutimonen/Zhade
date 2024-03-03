#include "NewApp.hpp"

#include "Buffer.hpp"
#include "StbImageResource.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <bit>

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

NewApp::NewApp(Config cfg)
    : m_camera{cfg.cameraDesc},
      m_mouseSensitivity{cfg.mouseSensitivity}
{
    initGLFW(cfg);
    initGLEW();
    initGL(cfg);
    initMisc();
}

//------------------------------------------------------------------------

void NewApp::run()
{
    while (not glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}

//------------------------------------------------------------------------

void NewApp::initGLFW(const Config& cfg)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(cfg.windowSize.x, cfg.windowSize.y, cfg.windowTitle.data(), nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hides the cursor on top of the window.
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, glfwKeyCallback);
    glfwSetCursorPosCallback(m_window, glfwMouseCallback);

    glfwSwapInterval(0);  // Uncapped FPS.

    glfwSetWindowUserPointer(m_window, this);  // May now obtain pointer to App via m_window.
}

//------------------------------------------------------------------------

void NewApp::initGLEW()
{
    glewExperimental = GL_TRUE;
    glewInit();
}

//------------------------------------------------------------------------

void NewApp::initGL(const Config& cfg)
{
    glDebugMessageCallback(glDebugCallback, nullptr);
    glm::vec3 clearColorNorm = glm::vec3{cfg.clearColor} / 255.0f;
    glClearColor(clearColorNorm.r, clearColorNorm.g, clearColorNorm.b, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &BufferUsage2Alignment[BufferUsage::UNIFORM]);
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &BufferUsage2Alignment[BufferUsage::STORAGE]);
}

//------------------------------------------------------------------------

void NewApp::initImGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
    ImGui::StyleColorsDark();
}

//------------------------------------------------------------------------

void NewApp::initMisc()
{
    stbi_set_flip_vertically_on_load(true);
}

//------------------------------------------------------------------------

void NewApp::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) [[unlikely]] {
        glfwSetWindowShouldClose(window, true);
    }

    NewApp* user = std::bit_cast<NewApp*>(glfwGetWindowUserPointer(window));
    user->m_keys[key] = action;

    user->m_camera.move();
}

//------------------------------------------------------------------------

void NewApp::glfwMouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    NewApp* user = std::bit_cast<NewApp*>(glfwGetWindowUserPointer(window));

    static float xPosPrev = xPos;
    static float yPosPrev = yPos;

    float deltaX = user->m_mouseSensitivity * (xPos - xPosPrev);
    float deltaY = user->m_mouseSensitivity * (yPosPrev - yPos);

    user->m_camera.rotate(deltaX, deltaY);

    xPosPrev = xPos;
    yPosPrev = yPos;
}

//------------------------------------------------------------------------

void NewApp::glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    const char* message, const void* userParam)
{
    if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH) [[unlikely]] {
        fmt::println("{}", message);
    }
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
