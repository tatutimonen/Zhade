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
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(windowWidth, windowHeight, name.c_str(), nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);

    // GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL
    glViewport(0, 0, windowWidth, windowHeight);
    // CSM: 15.0f/255.0f, 46.0f/255.0f, 101.0f/255.0f, 1.0f
    // Midnight Blue: 0.0f, 51.0f/255.0f, 102.0f/255.0f, 1.0f
    glClearColor(15.0f/255.0f, 46.0f/255.0f, 101.0f/255.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

//------------------------------------------------------------------------

void App::updateInternalTimes()
{
    float currentFrame = glfwGetTime();
    m_deltaTime = currentFrame - m_lastFrame;
    m_lastFrame = currentFrame;
}

//------------------------------------------------------------------------

void App::addMesh(std::shared_ptr<Mesh> mesh)
{
    m_meshes.push_back(mesh);
}

//------------------------------------------------------------------------

App::~App()
{
    glfwTerminate();
}

//------------------------------------------------------------------------
