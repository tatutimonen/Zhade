#include "App.hpp"


App* App::m_instance = new App();
bool App::m_keys[1024];
float App::m_theta = 0.0f;
float App::m_phi = 0.0f;

App* App::get_instance()
{
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
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, mouse_callback);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL
    GL_CALL(glViewport(0, 0, window_width, window_height));
}

void App::update_internal_times()
{
    float current_frame = glfwGetTime();
    m_delta_time = current_frame - m_last_frame;
    m_last_frame = current_frame;
}

App::~App()
{
    glfwTerminate();
}
