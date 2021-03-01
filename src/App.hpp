#pragma once

#include "Util.hpp"

extern "C" {
#include <GL/glew.h>
#include <GLFW/glfw3.h>
}
#include <utility>
#include <string>
#include <iostream>


class App {
public:
    App(const App&) = delete;
    App(App&&)      = delete;
    ~App();

    App& operator=(const App&) = delete;
    App& operator=(App&&)      = delete;

    inline static App& get_instance()     { static App instance; return instance; };
    inline GLFWwindow* get_gl_ctx() const { return m_window; }
    inline const bool* get_keys() const   { return m_keys; }
    inline float get_delta_time() const   { return m_delta_time; }
    inline float get_theta() const        { return m_theta; }
    inline float get_phi() const          { return m_phi; }

    void init();
    void update_internal_times();

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    
        m_keys[key] = (bool)action;
    }

    static void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
    {
        static float x_pos_prev = x_pos;
        static float y_pos_prev = y_pos;

        float x_offset = mouse_sensitivity * (x_pos - x_pos_prev);
        float y_offset = mouse_sensitivity * (y_pos_prev - y_pos);

        float theta_bound = glm::half_pi<float>() - 0.01f;
        m_theta = glm::clamp(m_theta + y_offset, -theta_bound, theta_bound);
        m_phi += x_offset;

        x_pos_prev = x_pos;
        y_pos_prev = y_pos;
    }

    const std::string name = "Shadow Mapper";
    static const unsigned int window_width = 800;
    static const unsigned int window_height = 800;

private:
    App() = default;

    static constexpr float mouse_sensitivity = 0.002f;
    static bool m_keys[512];
    static float m_theta;
    static float m_phi;

    GLFWwindow* m_window;
    float m_delta_time = 0.0f;
    float m_last_frame = 0.0f;
};
