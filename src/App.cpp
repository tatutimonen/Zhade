#include "App.hpp"


App* App::m_instance = nullptr;
bool App::m_keys[1024];

App* App::get_instance()
{
    if (m_instance == nullptr) {
        m_instance = new App();
        m_instance->init();
    }
    return m_instance;
}

void App::init()
{
    // GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    m_window = glfwCreateWindow(window_width, window_height, name.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, key_callback);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL
    GL_CALL(glViewport(0, 0, window_width, window_height));
}

App::~App()
{
    glfwTerminate();
}
