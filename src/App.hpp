#pragma once

#include "Texture2D.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cstdint>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

//------------------------------------------------------------------------

class Mesh;

//------------------------------------------------------------------------

class App {
public:
    App(const App&) = delete;
    App(App&&) = delete;
    ~App();

    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    inline static App& getInstance()                                   { static App instance; return instance; };
    inline GLFWwindow* getGlCtx() const                                { return m_window; }
    inline const bool* getKeys() const                                 { return m_keys; }
    inline float getDeltaTime() const                                  { return m_deltaTime; }
    inline float getPitch() const                                      { return m_pitch; }
    inline float getYaw() const                                        { return m_yaw; }
    inline const std::vector<std::shared_ptr<Mesh>>& getMeshes() const { return m_meshes; }

    void init();
    void updateInternalTimes();
    void addMesh(std::shared_ptr<Mesh> mesh);

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    
        m_keys[key] = static_cast<bool>(action);
    }

    static void mouseCallback(GLFWwindow* window, double xPos, double yPos)
    {
        static float xPosPrev = xPos;
        static float yPosPrev = yPos;

        float xOffset = mouseSensitivity * (xPos - xPosPrev);
        float yOffset = mouseSensitivity * (yPosPrev - yPos);

        float pitchBound = glm::half_pi<float>() - 0.01f;
        m_pitch = glm::clamp(m_pitch + yOffset, -pitchBound, pitchBound);
        m_yaw += xOffset;

        xPosPrev = xPos;
        yPosPrev = yPos;
    }

    const std::string name = "Shadow Mapper";
    static const uint32_t windowWidth = 800u;
    static const uint32_t windowHeight = 800u;
    uint32_t currentlyBoundBuffer = 0;
    uint32_t currentlyBoundVertexArray = 0;
    const std::shared_ptr<const Texture2D> defaultTexture2D = Texture2D::makeDefault();

private:
    App() = default;

    static constexpr float mouseSensitivity = 0.002f;
    static bool m_keys[512];
    static float m_pitch;
    static float m_yaw;

    GLFWwindow* m_window;
    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;

    std::vector<std::shared_ptr<Mesh>> m_meshes;
};

//------------------------------------------------------------------------