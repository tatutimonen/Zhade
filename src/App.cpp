#include "App.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

App::~App()
{
    glfwTerminate();
}

//------------------------------------------------------------------------

void App::init()
{
    // GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(s_windowWidth, s_windowHeight, name.data(), nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL
    glViewport(0, 0, s_windowWidth, s_windowHeight);
    // CSM: 15.0f/255.0f, 46.0f/255.0f, 101.0f/255.0f, 1.0f
    // Midnight Blue: 0.0f, 51.0f/255.0f, 102.0f/255.0f, 1.0f
    glClearColor(15.0f/255.0f, 46.0f/255.0f, 101.0f/255.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

//------------------------------------------------------------------------

void App::updateInternalTimes()
{
    float frameCurr = glfwGetTime();
    m_deltaTime = frameCurr - m_framePrev;
    m_framePrev = frameCurr;
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
