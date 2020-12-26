#include <iostream>

#include "gl_common.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL.h>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    auto window = glfwCreateWindow(800, 800, "Hello there.", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewExperimental = GL_TRUE;
    glewInit();

    GL_CALL(glViewport(0, 0, 800, 800));

    GLfloat vertices[] = {
        -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.0f,  0.0f,  0.0f
    };
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    // Vertex array object holds vertex attribute data, including possible element data.
    GLuint vao;
    GL_CALL(glGenVertexArrays(1, &vao));
    GLuint vbo;
    GL_CALL(glGenBuffers(1, &vbo));
    GLuint ebo;
    GL_CALL(glGenBuffers(1, &ebo));

    GL_CALL(glBindVertexArray(vao));
    {
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

        GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

        // Vertex positions.
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)0));
        GL_CALL(glEnableVertexAttribArray(0));
        // Texture coords.
        GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat))));
        GL_CALL(glEnableVertexAttribArray(1));
    }
    GL_CALL(glBindVertexArray(0));

    GLuint tex;
    GL_CALL(glGenTextures(1, &tex));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    int w, h;
    unsigned char* img = SOIL_load_image("../texture/cataphract.jpg", &w, &h, 0, SOIL_LOAD_RGB);
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    SOIL_free_image_data(img);
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    Shader* vertex_shader = new Shader(GL_VERTEX_SHADER, "../src/vshader.glsl");
    Shader* fragment_shader = new Shader(GL_FRAGMENT_SHADER, "../src/fshader.glsl");
    ShaderProgram* shader_program = new ShaderProgram(vertex_shader, fragment_shader);
    shader_program->use();

    GL_CALL(glBindVertexArray(vao));

    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();

        GL_CALL(glClearColor(0.4627f, 0.7255f, 0.0f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(window);

    }

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glDeleteVertexArrays(1, &vao));
    GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteBuffers(1, &ebo));
    shader_program->delete_shaders();
    delete shader_program;

    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}