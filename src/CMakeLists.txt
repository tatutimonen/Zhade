#-------------------------------------------------------------------------
# Source files.

set(src_files
    App.cpp
    AttributeData.cpp
    Buffer.cpp
    Camera.cpp
    PerspectiveCamera.cpp
    Shader.cpp
    ShaderProgram.cpp
    StbImageResource.cpp
    TextureStorage.cpp
    TextureView.cpp
    UniformBuffer.cpp
    VertexFormat.cpp
    common.cpp
    constants.cpp
    main.cpp
    util.cpp
)

#-------------------------------------------------------------------------
# Executable definition, library linking, etc.

add_executable(App ${src_files})

target_link_libraries(App
    PRIVATE GLEW::GLEW
            glfw
            glm::glm
            assimp::assimp
            imgui::imgui
)

#-------------------------------------------------------------------------
