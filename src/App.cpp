#include "App.hpp"

#include "Buffer.hpp"
#include "Camera.hpp"
#include "StbImageResource.hpp"
#include "util.hpp"

//------------------------------------------------------------------------

namespace Zhade
{

//------------------------------------------------------------------------

App::~App()
{
    glfwTerminate();
}

//------------------------------------------------------------------------

void App::init() const noexcept
{
    // GLFW.
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    m_window = glfwCreateWindow(s_windowWidth, s_windowHeight, s_title.data(), nullptr, nullptr);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, mouseCallback);
    glfwSetScrollCallback(m_window, Camera::scrollCallback);

    // GLEW.
    glewExperimental = GL_TRUE;
    glewInit();

    // OpenGL.
    glDebugMessageCallback(debugCallback, nullptr);
    glViewport(0, 0, s_windowWidth, s_windowHeight);
    glClearColor(15.0f/255.0f, 46.0f/255.0f, 101.0f/255.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Other.
    stbi_set_flip_vertically_on_load(1);

    GLint uniformBufferAlignment;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferAlignment);
    Buffer::s_alignmentTable.insert({ GL_UNIFORM_BUFFER, uniformBufferAlignment });

    GLint storageBufferAlignment;
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &storageBufferAlignment);
    Buffer::s_alignmentTable.insert({ GL_SHADER_STORAGE_BUFFER, storageBufferAlignment });
}

//------------------------------------------------------------------------

void App::updateTemporalState() const noexcept
{
    auto& [deltaTime, prevTime, FPS, frameTimes, writeIdx] = m_temporalState;

    const float currTime = glfwGetTime();
    deltaTime = currTime - prevTime;
    prevTime = currTime;

    const float frameTime = 1.0f / deltaTime;
    FPS[writeIdx] = frameTime;
    frameTimes[writeIdx] = deltaTime * 1000.0f;  // Seconds to milliseconds.
    writeIdx = (writeIdx + 1) % FPS.size();

    static float timer = 0.0f;
    timer += deltaTime;
    if (timer < 0.166f) return;  // Update performance info every 6th of a second.
    timer = 0.0f;

    const uint16_t averageFPS = static_cast<uint16_t>(util::average<float>(FPS));
    const float averageFrameTime = util::average<float>(frameTimes);
    glfwSetWindowTitle(m_window, std::format("Zhade - {} FPS, {:.2f} ms - ESC to quit", averageFPS, averageFrameTime).c_str());
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
