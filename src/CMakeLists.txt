#-------------------------------------------------------------------------
# Source files.

set(src_files
    App.cpp
    Buffer.cpp
    Camera.cpp
    PerspectiveCamera.cpp
    Shader.cpp
    ShaderProgram.cpp
    StbImageResource.cpp
    TextureStorage.cpp
    TextureView.cpp
    UniformBuffer.cpp
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
)

#-------------------------------------------------------------------------
