#include "AmbientLight.hpp"
#include "App.hpp"
#include "Camera.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "DirectionalLight.hpp"
#include "Mesh.hpp"
#include "MeshFactory.hpp"
#include "PerspectiveCamera.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "UniformBuffer.hpp"
#include "UniformBufferStorage.hpp"
#include "util.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <memory>

//------------------------------------------------------------------------

int main()
{
    App::get_instance().init();

    auto vshader = std::make_shared<Shader>(GL_VERTEX_SHADER, "lit.vert");
    auto fshader = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "lit.frag");
    auto shaderProgram = std::make_shared<ShaderProgram>(vshader, fshader);

    auto cameraSettings = std::make_unique<PerspectiveCamera::Settings>();
    auto camera = PerspectiveCamera(std::move(cameraSettings));

    // As described in http://devernay.free.fr/cours/opengl/materials.html
    auto ruby = std::make_shared<Mesh::Material>(
        Mesh::Material{
            glm::vec3(0.1745f,   0.01175f,  0.01175f),
            glm::vec3(0.61424f,  0.04136f,  0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            0.6f * 128.0f
        });
    auto cubeSettings = std::make_unique<Mesh::Settings>();
    cubeSettings->renderStrategy = shaderProgram;
    cubeSettings->material = ruby;
    cubeSettings->colorTexture = std::make_shared<Texture2D>(common::texturePath + "cataphract.jpg");
    auto cube = MeshFactory::makeCube(std::move(cubeSettings));
    cube->setTransformation(glm::scale(glm::vec3(2.0f)) * glm::translate(util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(cube));

    // As described in http://devernay.free.fr/cours/opengl/materials.html
    auto silver = std::make_shared<Mesh::Material>(
        Mesh::Material{
            glm::vec3(0.19225f),
            glm::vec3(0.50754f),
            glm::vec3(0.508273f),
            0.4f * 128.0f
        });
    auto emerald = std::make_shared<Mesh::Material>(
        Mesh::Material{
            glm::vec3(0.0215f, 0.1745f, 0.0215f),
            glm::vec3(0.07568f, 0.61424f, 0.07568f),
            glm::vec3(0.633f, 0.727811f, 0.633f),
            0.6f * 128.0f
        });
    auto planeSettings = std::make_unique<Mesh::Settings>();
    planeSettings->renderStrategy = shaderProgram;
    planeSettings->colorTexture = std::make_shared<Texture2D>(common::texturePath + "cataphract.jpg");
    //planeSettings->material = silver;
    auto plane = MeshFactory::makePlane(std::move(planeSettings));
    plane->setTransformation(glm::scale(glm::vec3(10.0f)) * glm::translate(-constants::Z_FIGHT_EPSILON * util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(plane));

    auto ambientLight = AmbientLight();
    auto directionalLight = std::make_shared<DirectionalLight>();

    auto pointLightsUniformBuffer = std::make_shared<UniformBuffer>(
        "PointLights",
        constants::POINT_LIGHT_BINDING,
        sizeof(PointLight::Settings)
    );
    auto pointLight = PointLight(UniformBufferStorage(pointLightsUniformBuffer, 0));

    while (!glfwWindowShouldClose(App::get_instance().get_gl_ctx()))
    {
        App::get_instance().update_internal_times();
        
        glfwPollEvents();
        camera.tick();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& mesh : App::get_instance().get_meshes())
            mesh->render();

        glfwSwapBuffers(App::get_instance().get_gl_ctx());
    }

    return 0;
}

//------------------------------------------------------------------------
