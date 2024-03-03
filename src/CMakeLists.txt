set(src_files
    App.cpp
    NewApp.cpp
    Buffer.cpp
    Camera.cpp
    NewCamera.cpp
    DirectionalLight.cpp
    Framebuffer.cpp
    Handle.cpp
    Model.cpp
    ObjectPool.cpp
    Pipeline.cpp
    Renderer.cpp
    ResourceManager.cpp
    Scene.cpp
    Stack.cpp
    StbImageResource.cpp
    Texture.cpp
    common.cpp
    main.cpp
    util.cpp
)

add_executable(${PROJECT_NAME} ${src_files})

target_link_libraries(${PROJECT_NAME}
    PRIVATE absl::span
            assimp::assimp
            fmt::fmt
            GLEW::GLEW
            glfw
            glm::glm
            imgui::imgui
            robin_hood::robin_hood
)
