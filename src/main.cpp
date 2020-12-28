#include <iostream>

#include "gl_common.hpp"
#include "App.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>


int main(void)
{
    
    App::get_instance()->init();
    PerspectiveCamera camera = PerspectiveCamera(glm::vec3(0.0f, 1.0f, 3.0f),
                                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                                 0.1f, 100.0f, 70.0f, 1.0f);

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

    glm::mat4 M(1.0f);
    M = glm::rotate(M, glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 1.0f));
    GLint M_loc = shader_program->get_uniform_location("M");
    GL_CALL(glUniformMatrix4fv(M_loc, 1, GL_FALSE, glm::value_ptr(M)));

    glm::mat4 V(1.0f);
    V = glm::translate(V, glm::vec3(0.0f, 0.0f, -1.5f));
    GLint V_loc = shader_program->get_uniform_location("V");
    GL_CALL(glUniformMatrix4fv(V_loc, 1, GL_FALSE, glm::value_ptr(V)));

    glm::mat4 P;
    P = glm::perspective(glm::radians(70.0f), 1.0f, 0.1f, 100.0f);
    GLint P_loc = shader_program->get_uniform_location("P");
    GL_CALL(glUniformMatrix4fv(P_loc, 1, GL_FALSE, glm::value_ptr(P)));

    int i = 0;
    while (!glfwWindowShouldClose(App::get_instance()->get_gl_ctx())) {
        
        glfwPollEvents();

        GL_CALL(glClearColor(0.4627f, 0.7255f, 0.0f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));

        glm::mat4 R(1.0f);
        R = glm::rotate(R, glm::radians(static_cast<GLfloat>(i++) * 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        GLint R_loc = shader_program->get_uniform_location("R");
        GL_CALL(glUniformMatrix4fv(R_loc, 1, GL_FALSE, glm::value_ptr(R)));

        GL_CALL(glBindVertexArray(vao));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        GL_CALL(glBindVertexArray(0));

        glfwSwapBuffers(App::get_instance()->get_gl_ctx());

    }

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glDeleteVertexArrays(1, &vao));
    GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteBuffers(1, &ebo));
    shader_program->delete_shaders();
    delete shader_program;

    return 0;
}
