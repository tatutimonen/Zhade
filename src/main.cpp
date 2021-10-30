#include "AmbientLight.hpp"
#include "App.hpp"
#include "Camera.hpp"
#include "Common.hpp"
#include "Mesh.hpp"
#include "MeshFactory.hpp"
#include "PerspectiveCamera.hpp"
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

int main()
{
    App::get_instance().init();

    auto vshader = std::make_shared<Shader>(GL_VERTEX_SHADER);
    auto fshader = std::make_shared<Shader>(GL_FRAGMENT_SHADER);
    auto shaderProgram = std::make_shared<ShaderProgram>(vshader, fshader);

    auto cameraSettings = std::make_unique<PerspectiveCamera::Settings>();
    auto camera = PerspectiveCamera(std::move(cameraSettings));

    auto cubeMaterial = std::shared_ptr<Mesh::Material>(new Mesh::Material{
        glm::vec3(0.0f),
        glm::vec3(0.1745f,   0.01175f,  0.01175f),
        glm::vec3(0.61424f,  0.04136f,  0.04136f),
        glm::vec3(0.727811f, 0.626959f, 0.626959f),
        0.6f
    });
    auto cubeSettings = std::make_unique<Mesh::Settings>();
    cubeSettings->renderStrategy = shaderProgram;
    cubeSettings->material = cubeMaterial;
    auto cube = MeshFactory::makeCube(std::move(cubeSettings));
    cube->setTransformation(glm::scale(glm::vec3(2.0f)) * glm::translate(Util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(cube));

    auto planeSettings = std::make_unique<Mesh::Settings>();
    planeSettings->renderStrategy = shaderProgram;
    planeSettings->colorTexture = std::make_shared<Texture2D>(Common::texturePath + "cataphract.jpg");
    auto plane = MeshFactory::makePlane(std::move(planeSettings));
    plane->setTransformation(glm::scale(glm::vec3(10.0f)) * glm::translate(-Util::zFightEpsilon * Util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(plane));

    auto ambientLight = AmbientLight(glm::vec4(1.0f));
    ambientLight.uploadAmbient(*shaderProgram);

    while (!glfwWindowShouldClose(App::get_instance().get_gl_ctx()))
    {
        App::get_instance().update_internal_times();
        
        glfwPollEvents();
        camera.tick(*shaderProgram);

        CHECK_GL_ERROR(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        for (const auto& mesh : App::get_instance().get_meshes())
            mesh->render();

        glfwSwapBuffers(App::get_instance().get_gl_ctx());
    }

    return 0;
}

//------------------------------------------------------------------------
