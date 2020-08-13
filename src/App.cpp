#include "App.hpp"

using std::string;
using std::runtime_error;

App::App(const string& name)
    : name_(name)
{
    setupWindow();
    setupExtensionWrangler();

    GLuint program = glCreateProgram();
    if (!program)
    {
        throw runtime_error("Error creating program!");
    }
    program_ = program;
}

void App::run()
{
    while (!glfwWindowShouldClose(window_))
    {
        ;
    }
}

void App::setupWindow()
{
    if (!glfwInit())
    {
        glfwTerminate();
        throw runtime_error("Error initializing GLFW!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Config::OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Config::OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, name_.c_str(), nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw runtime_error("Error creating window!");
    }

    window_ = window;
}

void App::setupExtensionWrangler()
{
    if (!window_)
    {
        throw runtime_error("Error initializing GLEW: context window missing!");
    }

    int buffer_width, buffer_height;
    glfwGetFramebufferSize(window_, &buffer_width, &buffer_height);
    glfwMakeContextCurrent(window_);
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(window_); window_ = 0;
        glfwTerminate();
        throw runtime_error("Error initializing GLEW!");
    }
    
    glViewport(0, 0, buffer_width, buffer_height);
}

void App::render()
{
    return;
}