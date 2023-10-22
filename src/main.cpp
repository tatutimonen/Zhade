#include "App.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Handle.hpp"
#include "Renderer.hpp"
#include "Model.hpp"
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
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <cmath>
#include <format>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>
#include <filesystem>
#include <memory_resource>

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    auto app = App();

    app.init();
    {
        ResourceManager mngr;

        auto scene = Scene({.mngr = &mngr});
        //scene.addModelFromFile(ASSET_PATH / "cornell" / "CornellBox-Empty-RG.obj");
        scene.addModelFromFile(ASSET_PATH / "sponza" / "sponza.obj");
        //scene.addModelFromFile(ASSET_PATH / "dragon" / "dragon.obj");

        const auto camera = Camera({.mngr = &mngr, .app = &app});

        const auto pipeline = Pipeline({
            .vertPath = SHADER_PATH / "debug.vert",
            .fragPath = SHADER_PATH / "debug.frag"
        });
        pipeline.bind();

        const auto renderer = Renderer({.mngr = &mngr, .scene = &scene});

        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            glfwPollEvents();
            camera.update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.getGLCtx());
        }

        /*for (const auto& cmd : std::span(renderer.commandBuffer()->getPtr<DrawElementsIndirectCommand>(),
                                         renderer.commandBuffer()->getByteSize() / sizeof(DrawElementsIndirectCommand)))
        {
            std::cout << std::format("vCount: {}, iCount: {}, firstIdx: {}, bVertex: {}, baseInst: {}\n",
                cmd.vertexCount, cmd.instanceCount, cmd.firstIndex, cmd.baseVertex, cmd.baseInstance);
        }*/
        /*for (const auto& vtx : std::span(scene.vertexBuffer()->getPtr<Vertex>(),
                                         scene.vertexBuffer()->getByteSize() / sizeof(Vertex)))
        {
            std::cout << glm::to_string(vtx.pos) << "\n";
        }
        for (const auto& idx : std::span(scene.indexBuffer()->getPtr<GLuint>(),
                                         scene.indexBuffer()->getByteSize() / sizeof(GLuint)))
        {
            std::cout << idx << "\n";
        }
        std::cout << "\n";*/
        for (const auto han : std::span(renderer.textureBuffer()->getPtr<GLuint64>(),
                                        renderer.textureBuffer()->getByteSize() / sizeof(GLuint64)))
        {
            std::cout << han << "\n";
        }
        std::cout << "\n";
        std::cout << BufferUsage2Alignment[BufferUsage::STORAGE] << "\n";
        std::cout << mngr.get(scene.m_defaultTexture)->getHandle() << "\n";
        std::cout << renderer.commandBuffer()->getByteSize() / sizeof(DrawElementsIndirectCommand) << "\n";
        std::cout << renderer.textureBuffer()->getByteSize() / sizeof(GLuint64) << "\n";
        std::cout << _MSC_VER << std::endl;
    }

    return 0;
}

//------------------------------------------------------------------------
