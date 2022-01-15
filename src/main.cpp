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

    auto vshader = ShaderImpl(GL_VERTEX_SHADER, "lit.vert");
    auto fshader = ShaderImpl(GL_FRAGMENT_SHADER, "lit.frag");
    auto shaderProgram = std::make_shared<ShaderProgram>(vshader, fshader);

    auto cameraSettings = std::make_unique<PerspectiveCamera::Settings>();
    auto camera = PerspectiveCamera(std::move(cameraSettings));

    auto cubeSettings = Mesh::Settings();
    // As described in http://devernay.free.fr/cours/opengl/materials.html
    auto ruby = std::make_shared<Mesh::Material>(
        Mesh::Material{
            glm::vec3(0.1745f,   0.01175f,  0.01175f),
            glm::vec3(0.61424f,  0.04136f,  0.04136f),
            glm::vec3(0.727811f, 0.626959f, 0.626959f),
            0.6f
        });
    cubeSettings.material = ruby;
    auto cube = MeshFactory::makeCube(cubeSettings);
    cube->setModelMatrix(glm::scale(glm::vec3(2.0f)) * glm::translate(util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(cube));

    auto planeSettings = Mesh::Settings();
    // As described in http://devernay.free.fr/cours/opengl/materials.html
    auto silver = std::make_shared<Mesh::Material>(
        Mesh::Material{
            glm::vec3(0.19225f),
            glm::vec3(0.50754f),
            glm::vec3(0.508273f),
            0.4f
        });
    planeSettings.material = silver;
    auto plane = MeshFactory::makePlane(planeSettings);
    plane->setModelMatrix(glm::scale(glm::vec3(10.0f)) * glm::translate(-constants::Z_FIGHT_EPSILON * util::makeUnitVec3y()));
    App::get_instance().add_mesh(std::move(plane));

    auto ambientLight = AmbientLight();
    auto directionalLight = std::make_shared<DirectionalLight>();

    auto pointLightsUniformBuffer = std::make_shared<UniformBuffer>(
        "PointLights",
        constants::POINT_LIGHT_BINDING,
        sizeof(PointLight::Settings)
    );
    auto pointLight = PointLight(UniformBufferStorage(pointLightsUniformBuffer, 0));

    std::cout << glGetString(GL_VENDOR) << "\n";
    std::cout << glGetString(GL_RENDERER) << "\n";
    std::cout << glGetString(GL_VERSION) << "\n";
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    shaderProgram->use();

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
