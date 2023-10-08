#include "App.hpp"
#include "Buffer.hpp"
#include "Camera.hpp"
#include "Handle.hpp"
#include "IndirectRenderer.hpp"
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
        scene.addModelFromFile(ASSET_PATH / "sponza" / "sponza.obj");
        std::cout << scene.vertexBuffer()->getByteSize() << "\n";

        const auto camera = Camera({.mngr = &mngr, .app = &app});

        const auto pipeline = Pipeline({
            .vertPath = SHADER_PATH / "debug.vert",
            .fragPath = SHADER_PATH / "debug.frag"
        });
        pipeline.bind();

        const auto renderer = IndirectRenderer({.mngr = &mngr, .scene = &scene});

        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            glfwPollEvents();
            camera.update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.getGLCtx());
        }

        std::cout << renderer.commandBuffer()->getByteSize() << "\n";
        std::cout << renderer.transformBuffer()->getByteSize() << "\n";
        std::cout << renderer.textureBuffer()->getByteSize() << "\n";
    }

    return 0;
}

//------------------------------------------------------------------------
