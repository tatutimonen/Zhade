#-------------------------------------------------------------------------
# Source files.

set(src_files
    App.cpp
    Buffer.cpp
    Camera.cpp
    Handle.cpp
    ObjectPool.cpp
    PerspectiveCamera.cpp
    ResourceManager.cpp
    Shader.cpp
    ShaderProgram.cpp
    Stack.cpp
    StbImageResource.cpp
    TextureStorage.cpp
    TextureView.cpp
    common.cpp
    constants.cpp
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
