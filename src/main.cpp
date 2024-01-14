#include "App.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    ResourceManager mngr{};

    App app{};
    app.init();
    {
        Renderer renderer{{
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
                        .fragPath = SHADER_PATH / "passthrough.frag",
                        .compPath = SHADER_PATH / "populateBuffers.comp"
                    }
                }
            },
            .cameraDesc = {
                .mngr = &mngr,
                .app = &app
            },
            .mainPassDesc = {
                .vertPath = SHADER_PATH / "main.vert",
                .fragPath = SHADER_PATH / "main.frag"
            }
        }};

        renderer.scene().addModelFromFile(ASSET_PATH / "crytek-sponza" / "sponza.obj");

        while (not glfwWindowShouldClose(app.getGLCtx()))
        {
            glfwPollEvents();
            renderer.camera().update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.getGLCtx());
        }
    }

    return 0;
}

//------------------------------------------------------------------------
