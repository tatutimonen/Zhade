#pragma once

#include "Mesh.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

//------------------------------------------------------------------------

class App {
public:
    App(const App&) = delete;
    App(App&&) = delete;
    ~App();

    App& operator=(const App&) = delete;
    App& operator=(App&&) = delete;

    inline static App& get_instance()                                   { static App instance; return instance; };
    inline GLFWwindow* get_gl_ctx() const                               { return m_window; }
    inline const bool* get_keys() const                                 { return m_keys; }
    inline float get_delta_time() const                                 { return m_delta_time; }
    inline float get_pitch() const                                      { return m_pitch; }
    inline float get_yaw() const                                        { return m_yaw; }
    inline const std::vector<std::shared_ptr<Mesh>>& get_meshes() const { return m_meshes; }

    void init();
    void update_internal_times();
    void add_mesh(std::shared_ptr<Mesh> mesh);

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

        float pitch_bound = glm::half_pi<float>() - 0.01f;
        m_pitch = glm::clamp(m_pitch + y_offset, -pitch_bound, pitch_bound);
        m_yaw += x_offset;

        x_pos_prev = x_pos;
        y_pos_prev = y_pos;
    }

    const std::string name = "Shadow Mapper";
    static const unsigned int window_width = 800u;
    static const unsigned int window_height = 800u;

private:
    App() = default;

    static constexpr float mouse_sensitivity = 0.002f;
    static bool m_keys[512];
    static float m_pitch;
    static float m_yaw;

    GLFWwindow* m_window;
    float m_delta_time = 0.0f;
    float m_last_frame = 0.0f;

    std::vector<std::shared_ptr<Mesh>> m_meshes;
};

//------------------------------------------------------------------------