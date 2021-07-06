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

#include <future>
#include <memory>
#include <utility>

//------------------------------------------------------------------------

int main(void)
{
    App::get_instance().init();

    auto vshader = std::make_shared<Shader>(GL_VERTEX_SHADER, Common::shaderPath + "default.vert");
    auto fshader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, Common::shaderPath + "default.frag");
    auto shader_program = std::make_shared<ShaderProgram>(vshader, fshader);

    auto camera_spec = std::make_unique<PerspectiveCamera::Specification>();
    auto camera = PerspectiveCamera(std::move(camera_spec));
    camera.pushViewMatrix(*shader_program);
    camera.pushProjectionMatrix(*shader_program);

    auto cube_material = std::shared_ptr<Mesh::Material>(new Mesh::Material{
        glm::vec3(0.0f),
        glm::vec3(0.1745f,   0.01175f,  0.01175f),
        glm::vec3(0.61424f,  0.04136f,  0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    });
    auto cube = Mesh::makeCube(shader_program, cube_material);
    App::get_instance().add_mesh(std::move(cube));

    // The 0x0502 error related to meshes might be due to the singleton nature of App.
    // Mesh list should anyways be given to a separate renderer object, not App, so w/e for now.
    auto plane = Mesh::makePlane(
        shader_program, std::make_shared<Mesh::Material>(), std::make_shared<Texture2D>(Common::texturePath + "cataphract.jpg")
    );
    plane->setTransformation(
        glm::scale(glm::vec3(10.0f)) * glm::translate(-Util::zFightEpsilon * Util::makeUnitVec3y())
    );
    App::get_instance().add_mesh(std::move(plane));

    while (!glfwWindowShouldClose(App::get_instance().get_gl_ctx())) {
        App::get_instance().update_internal_times();
        
        glfwPollEvents();
        const bool moved = camera.move();
        const bool rotated = camera.rotate();
        if (moved || rotated) {
            camera.pushViewMatrix(*shader_program);
            camera.pushProjectionMatrix(*shader_program);
        }

        GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (const auto& mesh : App::get_instance().get_meshes())
            mesh->render();

        glfwSwapBuffers(App::get_instance().get_gl_ctx());
    }

    return 0;
}

//------------------------------------------------------------------------
