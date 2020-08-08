#pragma once

#include "Config.hpp"

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace
{
    GLuint constexpr WINDOW_WIDTH  = 800;
    GLuint constexpr WINDOW_HEIGHT = 600;
}

class App
{
public:
                            App(std::string const& name);
    virtual                 ~App() {}

    void                    run();

private:
                            App(App const&);
    App&                    operator=(App const&);

    int                     setupWindow();
    int                     setupExtensionWrangler();
    void                    render();

    std::string const&      name_;
    GLFWwindow*             window_ = nullptr;
    GLuint                  program_;
    GLuint                  static_vao_ = 0;
    GLuint                  dynamic_vao_ = 0;
    GLuint                  static_vbo_ = 0;
    GLuint                  dynamic_vbo_ = 0;
};