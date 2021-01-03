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
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>
#include <assimp/Importer.hpp>

#include <utility>
#include <memory>


int main(void)
{
    App::get_instance()->init();
    auto camera = PerspectiveCamera(glm::vec3(0.0f, 0.0f, 3.0f));

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

    auto vertex_shader = std::shared_ptr<Shader>(new Shader(GL_VERTEX_SHADER, "../src/vshader.glsl"));
    auto fragment_shader = std::shared_ptr<Shader>(new Shader(GL_FRAGMENT_SHADER, "../src/fshader.glsl"));
    auto shader_program = std::unique_ptr<ShaderProgram>(new ShaderProgram(vertex_shader, fragment_shader));
    shader_program->use();

    glm::mat4 model_matrix(1.0f);
    GLint model_matrix_loc = shader_program->get_uniform_location("model");
    GL_CALL(glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, glm::value_ptr(model_matrix)));
    camera.push_view_matrix(shader_program->get_uniform_location("view"));
    camera.push_projection_matrix(shader_program->get_uniform_location("projection"));

    while (!glfwWindowShouldClose(App::get_instance()->get_gl_ctx())) {
        
        App::get_instance()->update_internal_times();
        
        glfwPollEvents();
        const bool moved = camera.move();
        const bool rotated = camera.rotate();
        if (moved || rotated) {
            camera.push_view_matrix(shader_program->get_uniform_location("view"));
            camera.push_projection_matrix(shader_program->get_uniform_location("projection"));
        }

        GL_CALL(glClearColor(0.4627f, 0.7255f, 0.0f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        GL_CALL(glBindTexture(GL_TEXTURE_2D, tex));

        GL_CALL(glBindVertexArray(vao));
        GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        GL_CALL(glBindVertexArray(0));

        glfwSwapBuffers(App::get_instance()->get_gl_ctx());

    }

    GL_CALL(glBindVertexArray(0));
    GL_CALL(glDeleteVertexArrays(1, &vao));
    GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteBuffers(1, &ebo));

    return 0;
}
