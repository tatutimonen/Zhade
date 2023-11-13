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
        //scene.addModelFromFile(ASSET_PATH / "cornell" / "CornellBox-Original.obj");
        scene.addModelFromFile(ASSET_PATH / "crytek-sponza" / "sponza.obj");
        //scene.addModelFromFile(ASSET_PATH / "dragon" / "dragon.obj");

        const auto camera = Camera({.mngr = &mngr, .app = &app});

        const auto pipeline = Pipeline({
            .vertPath = SHADER_PATH / "main.vert",
            .fragPath = SHADER_PATH / "main.frag"
        });
        pipeline.bind();

        const auto renderer = Renderer({.mngr = &mngr, .scene = &scene});

        renderer.processSceneGraph();
        while (!glfwWindowShouldClose(app.getGLCtx()))
        {
            glfwPollEvents();
            camera.update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.getGLCtx());
        }
        std::println("{}", glm::to_string(camera.getCenter()));
        std::println("{}", glm::to_string(camera.getTarget()));
        std::println("{}", glm::to_string(camera.getUp()));
    }

    return 0;
}

//------------------------------------------------------------------------
