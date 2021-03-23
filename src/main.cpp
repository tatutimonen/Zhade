#include "App.hpp"
#include "Camera.hpp"
#include "Common.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "Util.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>

#include <utility>
#include <memory>


int main(void)
{
    App::get_instance().init();

    auto vshader = std::make_shared<Shader>(GL_VERTEX_SHADER, Common::shader_path + "vshader_default.glsl");
    auto fshader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, Common::shader_path + "fshader.glsl");
    auto shader_program = std::make_shared<ShaderProgram>(vshader, fshader);

    auto camera_spec = std::make_unique<PerspectiveCamera::Specification>();
    auto camera = PerspectiveCamera(std::move(camera_spec));
    camera.push_view_matrix(*shader_program.get());
    camera.push_projection_matrix(*shader_program.get());


    auto cube_material = std::make_shared<Mesh::Material>(
        glm::vec3(0.1745f,   0.01175f,  0.01175f),
        glm::vec3(0.61424f,  0.04136f,  0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    );
    auto cube = Mesh::make_cube(shader_program, cube_material);
    App::get_instance().add_mesh(std::move(cube));

    auto plane = Mesh::make_plane(shader_program);
    plane->set_transformation(glm::scale(glm::vec3(10.0f)) * glm::translate(glm::vec3(0.0f, -Util::z_fight_epsilon, 0.0f)));
    App::get_instance().add_mesh(std::move(plane));

    while (!glfwWindowShouldClose(App::get_instance().get_gl_ctx())) {

        App::get_instance().update_internal_times();
        
        glfwPollEvents();
        const bool moved = camera.move();
        const bool rotated = camera.rotate();
        if (moved || rotated) {
            camera.push_view_matrix(*shader_program.get());
            camera.push_projection_matrix(*shader_program.get());
        }

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (const auto& mesh : App::get_instance().get_meshes())
            mesh->render();

        glfwSwapBuffers(App::get_instance().get_gl_ctx());

    }

    return 0;
}
