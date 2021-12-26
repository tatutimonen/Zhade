#include "App.hpp"

//------------------------------------------------------------------------

// The elegance of singletons :)
bool App::m_keys[512];
float App::m_pitch = 0.0f;
float App::m_yaw = -glm::half_pi<float>();

//------------------------------------------------------------------------

void App::init()
{
    // GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    glViewport(0, 0, window_width, window_height);
    glClearColor(46/255.0f, 68/255.0f, 130/255.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

//------------------------------------------------------------------------

void App::update_internal_times()
{
    float current_frame = glfwGetTime();
    m_delta_time = current_frame - m_last_frame;
    m_last_frame = current_frame;
}

//------------------------------------------------------------------------

void App::add_mesh(std::shared_ptr<Mesh> mesh)
{
    m_meshes.push_back(mesh);
}

//------------------------------------------------------------------------

App::~App()
{
    glfwTerminate();
}

//------------------------------------------------------------------------
