#include "App.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "ShaderProgram.hpp"
#include "StbImageResource.hpp"
#include "Texture.hpp"
#include "common.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "ObjectPool.hpp"

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
#include <filesystem>

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    auto app = App();

    app.init();
    {
        Assimp::Importer importer{};
        const aiScene* scene = importer.ReadFile((ASSET_PATH / "sponza" / "sponza.obj").string().c_str(), constants::ASSIMP_LOAD_FLAGS);
        if (scene == nullptr)
        {
            std::cerr << importer.GetErrorString() << "\n";
            // TODO: Get some sort of default geometry instead.
        }

        std::filesystem::path path1 = "foo";
        std::filesystem::path path2 = "bar";
        std::cout << (path1 / path2.concat("asd.c\n")).parent_path().string();

        std::cout << std::format("numMeshes: {}, numMaterirals: {}\n", scene->mNumMeshes, scene->mNumMaterials);
        std::cout << Buffer::s_alignmentTable.at(GL_UNIFORM_BUFFER) << " " << Buffer::s_alignmentTable.at(GL_SHADER_STORAGE_BUFFER) << "\n";

        for (uint32_t i : std::views::iota(0u, scene->mNumMaterials))
        {
            const aiMaterial* material = scene->mMaterials[i];

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_FAILURE) continue;
                std::cout << std::format("{}\n", path.data);
            }
        }

        ResourceManager mngr;

        auto shaderProgram = ShaderProgram(
            Shader<GL_VERTEX_SHADER>(SHADER_PATH / "debug.vert"),
            Shader<GL_FRAGMENT_SHADER>(SHADER_PATH / "debug.frag")
        );

        const auto camera = Camera(&mngr, &app);

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

        const Renderer renderer {
            &mngr,
            {
                .vertexBuffer = mngr.createBuffer(GL_ARRAY_BUFFER, 1 << 16),
                .indexBuffer = mngr.createBuffer(GL_ELEMENT_ARRAY_BUFFER, 1 << 16),
                .program = &shaderProgram
            }
        };

        std::vector<glm::mat3x4> modelMatrices;
        for (auto i = 0u; i < numQuads; ++i)
        {
            modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3(0.0f, (float)i, 0.0f)))));
        }
        for (auto i = 0u; i < numTris; ++i)
        {
            modelMatrices.push_back(glm::mat3x4(glm::transpose(glm::translate(glm::vec3((float)(i+1), std::sinf(2*i), 0.0f)))));
        }

        auto quadTextures = std::array<Handle<Texture>, numQuads>{
            Texture::fromFile(&mngr, TEXTURE_PATH / "cataphract.jpg"),
            Texture::fromFile(&mngr, TEXTURE_PATH / "berserk.png"),
            Texture::fromFile(&mngr, TEXTURE_PATH / "longbowman.png"),
            Texture::fromFile(&mngr, TEXTURE_PATH / "jaguarwarrior.png")
        };

        renderer.submit({
            .model = quadModel,
            .instanceCount = numQuads,
            .transformations = std::span(modelMatrices.begin(), modelMatrices.begin() + numQuads),
            .textures = quadTextures
        });
        renderer.submit({
            .model = triModel,
            .instanceCount = numTris,
            .transformations = std::span(modelMatrices.begin() + numQuads, modelMatrices.end()),
            .textures = quadTextures
        });

        glfwSwapInterval(0);

        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            app.updateTemporalState();
            glfwPollEvents();
            camera.update();
            renderer.render();
            glfwSwapBuffers(app.getGLCtx());
        }
    }

    return 0;
}

//------------------------------------------------------------------------
