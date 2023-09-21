#include "App.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Handle.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Pipeline.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"
#include "StbImageResource.hpp"
#include "Texture.hpp"
#include "common.hpp"
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
        /*Assimp::Importer importer{};
        const aiScene* scene = importer.ReadFile((ASSET_PATH / "sponza" / "sponza.obj").string().c_str(), ASSIMP_LOAD_FLAGS);
        if (scene == nullptr)
        {
            std::cerr << importer.GetErrorString() << "\n";
            // TODO: Get some sort of default geometry instead.
        }

        std::cout << std::format("numMeshes: {}, numMaterirals: {}\n", scene->mNumMeshes, scene->mNumMaterials);
        std::cout << BufferUsage2Alignment[BufferUsage::UNIFORM] << " " << BufferUsage2Alignment[BufferUsage::STORAGE] << "\n";

        for (uint32_t i : std::views::iota(0u, scene->mNumMaterials))
        {
            const aiMaterial* material = scene->mMaterials[i];

            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
            {
                aiString path;
                if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_FAILURE) continue;
                std::cout << std::format("{}\n", path.data);
            }
        }*/

        ResourceManager mngr;

        const auto pipeline = Pipeline({
            .vertPath = SHADER_PATH / "debug.vert",
            .fragPath = SHADER_PATH / "debug.frag"
        });
        pipeline.bind();

        const auto scene2 = Scene({.mngr = &mngr});
        scene2.addModelFromFile(ASSET_PATH / "dragon" / "dragon.obj");
        scene2.addModelFromFile(ASSET_PATH / "sponza" / "sponza.obj");
        scene2.addModelFromFile(ASSET_PATH / "dragon" / "dragon.obj");
        for (const auto& modelHandle : scene2.getModels())
        {
            std::cout << mngr.get(modelHandle)->getId() << "\n";
        }

        const auto camera = Camera({.mngr = &mngr, .app = &app});

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
                .vertexBuffer = mngr.createBuffer({.byteSize = 1 << 16, .usage = BufferUsage::VERTEX}),
                .indexBuffer = mngr.createBuffer({.byteSize = 1 << 16, .usage = BufferUsage::INDEX})
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
            glfwPollEvents();
            camera.update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.getGLCtx());
        }
    }

    return 0;
}

//------------------------------------------------------------------------
