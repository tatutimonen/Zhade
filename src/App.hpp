#pragma once

#include "gl_common.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>


class App {
public:
    App(const App&) = delete;
    App(App&&) = delete;
    ~App();

    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    static App* get_instance();
    GLFWwindow* get_gl_ctx() const { return m_window; }
    const bool* get_keys() const   { return m_keys; }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (key >= 0 && key < 1024) {
            if (action == GLFW_PRESS)
                m_keys[key] = true;
            else if (action == GLFW_RELEASE)
                m_keys[key] = false;
        }
    }


    const std::string name = "Shadow Mapper";
    const unsigned window_width = 800;
    const unsigned window_height = 800;

private:
    App() = default;

    void init();


    static App* m_instance;
    GLFWwindow* m_window;
    static bool m_keys[1024];
};
