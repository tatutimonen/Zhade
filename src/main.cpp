#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto window = glfwCreateWindow(800, 600, "Hello there.", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewExperimental = GL_TRUE;
    glewInit();

    glViewport(0, 0, 800, 600);

    GLfloat vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.0f,  0.0f,
         0.5f,  1.0f,  0.0f,
         1.0f,  0.0f,  0.0f 
    };
    GLuint vao;
    glGenVertexArrays(1, &vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const GLchar* vertex_shader_source = "\
        #version 330\n\
        \n\
        layout (location = 0) in vec3 position;\n\
        \n\
        void main()\n\
        {\n\
            gl_Position = vec4(position, 1.0);\n\
        }\n\
    ";
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragment_shader_source = "\
        #version 330 core\n\
        \n\
        out vec4 color;\n\
        \n\
        void main()\n\
        {\n\
            color = vec4(1.0, 0.0, 0.0, 1.0);\n\
        }\n\
    ";
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    GLuint shader_program;
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    GLuint fragment_shader_2;
    fragment_shader_2 = glCreateShader(GL_FRAGMENT_SHADER);
    const GLchar* fragment_shader_source_2 = "\
        #version 330 core\n\
        \n\
        out vec4 color;\n\
        \n\
        void main()\n\
        {\n\
            color = vec4(1.0, 1.0, 0.0, 1.0);\n\
        }\n\
    ";
    glShaderSource(fragment_shader_2, 1, &fragment_shader_source_2, nullptr);
    glCompileShader(fragment_shader_2);

    GLuint shader_program_2;
    shader_program_2 = glCreateProgram();
    glAttachShader(shader_program_2, vertex_shader);
    glAttachShader(shader_program_2, fragment_shader_2);
    glLinkProgram(shader_program_2);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();

        glClearColor(0.4627f, 0.7255f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(vao);
        glUseProgram(shader_program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(shader_program_2);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);

    }

    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}