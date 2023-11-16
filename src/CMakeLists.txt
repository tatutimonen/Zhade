#-------------------------------------------------------------------------
# Source files.

set(src_files
    App.cpp
    Buffer.cpp
    Camera.cpp
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

#-------------------------------------------------------------------------
# Executable definition, library linking, etc.

add_executable(${PROJECT_NAME} ${src_files})
target_link_libraries(${PROJECT_NAME}
    PRIVATE GLEW::GLEW
            glfw
            glm::glm
            assimp::assimp
            imgui::imgui
            robin_hood::robin_hood
)

#-------------------------------------------------------------------------
