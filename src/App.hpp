#pragma once

#include "Config.hpp"

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace
{
constexpr GLuint WINDOW_WIDTH  = 800;
constexpr GLuint WINDOW_HEIGHT = 600;
} // namespace

class App
{
public:
                            App(const std::string& name);
    virtual                 ~App() {}

    void                    run();

private:
    //                        App(const App&);
    //App&                    operator=(const App&);

    void                     setupWindow();
    void                     setupExtensionWrangler();
    void                    render();

    const std::string&      name_;
    GLFWwindow*             window_ = nullptr;
    GLuint                  program_;
    GLuint                  static_vao_ = 0;
    GLuint                  dynamic_vao_ = 0;
    GLuint                  static_vbo_ = 0;
    GLuint                  dynamic_vbo_ = 0;
};