#include "App.hpp"
#include "Buffer.hpp"
#include "Model.hpp"
#include "PerspectiveCamera.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "TextureStorage.hpp"
#include "TextureView.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "StbImageResource.hpp"
#include "util.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <array>
#include <cmath>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    auto app = App();

    app.init();
    {
        ResourceManager mngr;

        auto shaderProgram = ShaderProgram(
            Shader<GL_VERTEX_SHADER>(SHADER_PATH + "debug.vert"),
            Shader<GL_FRAGMENT_SHADER>(SHADER_PATH + "debug.frag")
        );

        const auto camera = PerspectiveCamera(&mngr, &app);

        static constexpr auto numQuads = 4;
        static constexpr auto numTris = 100;

        // Basic vertex data setup.

        Vertex quadVerts[] = {
            { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
            { glm::vec3(-0.5f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f) },
            { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
        };
        GLuint quadInds[] = {
            0, 1, 2,
            2, 3, 0
        };
        const auto quadModel = mngr.createModel(std::span<Vertex>(quadVerts), std::span<GLuint>(quadInds));

        Vertex triVerts[] = {
            { glm::vec3( 0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
            { glm::vec3( 0.0f,  0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f) },
            { glm::vec3(-0.5f,  0.0f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) }
        };
        GLuint triInds[] = {
            0, 1, 2
        };
        const auto triModel = mngr.createModel(std::span<Vertex>(triVerts), std::span<GLuint>(triInds));

        const Renderer renderer{
            &mngr,
            {
                .vertexBuffer = mngr.createBuffer(GL_ARRAY_BUFFER, 1 << 16),
                .indexBuffer = mngr.createBuffer(GL_ELEMENT_ARRAY_BUFFER, 1 << 16),
                .program = &shaderProgram
            }
        };

        // Create render commands and gather model matrices.

        std::vector<glm::mat3x4> modelMatrices;
        for (auto i = 0u; i < numQuads; ++i)
        {
            modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3(0.0f, (float)i, 0.0f)))));
        }
        for (auto i = 0u; i < numTris; ++i)
        {
            modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3((float)(i+1), std::sinf(2*i), 0.0f)))));
        }

        renderer.submit({
            .model = quadModel,
            .instanceCount = numQuads,
            .transformations = std::span(modelMatrices.begin(), modelMatrices.begin() + numQuads)
        });
        renderer.submit({
            .model = triModel,
            .instanceCount = numTris,
            .transformations = std::span(modelMatrices.begin() + numQuads, modelMatrices.end())
        });

        const auto texStorage = TextureStorage();

        const auto& cataViewOpt = texStorage.pushDataFromFile(TEXTURE_PATH + "cataphract.jpg");
        const auto& berserkViewOpt = texStorage.pushDataFromFile(TEXTURE_PATH + "berserk.png");
        const auto& longbowViewOpt = texStorage.pushDataFromFile(TEXTURE_PATH + "longbowman.png");
        const auto& jagViewOpt = texStorage.pushDataFromFile(TEXTURE_PATH + "jaguarwarrior.png");

        texStorage.bindToUnit(0);
        texStorage.generateMipmap();

        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            app.updateInternalTimes();
            glfwPollEvents();
            camera.tick();
            renderer.render();
            glfwSwapBuffers(app.getGLCtx());
        }
    }

    return 0;
}

//------------------------------------------------------------------------
