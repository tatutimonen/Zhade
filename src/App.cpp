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
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
    glfwSetScrollCallback(m_window, Camera<>::scrollCallback);

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
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &BufferUsage2Alignment[BufferUsage::UNIFORM]);
    glGetIntegerv(GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT, &BufferUsage2Alignment[BufferUsage::STORAGE]);

    // stb.
    stbi_set_flip_vertically_on_load(1);

    // ImGui.
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
    ImGui::StyleColorsDark();
}

//------------------------------------------------------------------------

void App::updateAndRenderGUI() const noexcept
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Foo");
    ImGui::Text("Bar");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

//------------------------------------------------------------------------

}  // namespace Zhade

//------------------------------------------------------------------------
