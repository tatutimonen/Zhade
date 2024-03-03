#include "App.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include "ResourceManager.hpp"

//------------------------------------------------------------------------

int main()
{
    using namespace Zhade;

    ResourceManager mngr;

    App app;
    app.init();
    {
        Renderer renderer{{
            .mngr = &mngr,
            .sceneDesc = {
                .mngr = &mngr,
                .sunLightDesc = {
                    .mngr = &mngr,
                    .props = {
                        .direction = glm::vec3{0.273005, -0.960278, 0.057737},
                        .strength = 1.0f,
                        .color = {1.0f, 1.0f, 1.0f},
                        .ambient = {0.4f, 0.4f, 0.4f}
                    },
                    .shadowMapDims = {2048, 2048},
                    .shadowPassDesc = {
                        .vertPath = SHADER_PATH / "shadowMap.vert",
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

        while (not glfwWindowShouldClose(app.glCtx()))
        {
            glfwPollEvents();
            renderer.camera().update();
            renderer.render();
            app.updateAndRenderGUI();
            glfwSwapBuffers(app.glCtx());
        }
    }

    return 0;
}

//------------------------------------------------------------------------
