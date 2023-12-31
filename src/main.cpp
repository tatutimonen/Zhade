#include "App.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    App app{};
    app.init();
    {
        ResourceManager mngr;
        const auto renderer = Renderer({
            .mngr = &mngr,
            .sceneDesc = {
                .mngr = &mngr,
                .sunLightDesc = {
                    .mngr = &mngr,
                    .props = {
                        .direction = {1600.0f, 5300.0f, 920.0f},
                        .strength = 1.0f,
                        .color = {1.0f, 1.0f, 1.0f},
                        .ambient = {1.0f, 1.0f, 1.0f}
                    },
                    .shadowMapDims = {1024, 1024},
                    .shadowPassDesc = {
                        .vertPath = SHADER_PATH / "shadowMapDirectional.vert",
                        .fragPath = SHADER_PATH / "passthrough.frag"
                    }
                }
            },
            .mainPipelineDesc = {
                .vertPath = SHADER_PATH / "main.vert",
                .fragPath = SHADER_PATH / "main.frag",
                .compPath = SHADER_PATH / "populateBuffers.comp",
                .managed = false
            }
        });
        renderer.scene().addModelFromFile(ASSET_PATH / "crytek-sponza" / "sponza.obj");

        const auto camera = Camera({
            .uniformBuffer = renderer.viewProjBufferHandle(),
            .mngr = &mngr,
            .app = &app
        });

        while (not glfwWindowShouldClose(app.getGLCtx()))
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
