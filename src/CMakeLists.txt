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
    Texture.cpp
    Texture2D.cpp
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

set_target_properties(App PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_link_libraries(App
    PRIVATE libglew_static
            glfw
            glm
            stb_image
            assimp
            imgui
)
target_include_directories(App
    PRIVATE ${lib_dir}/glew/include
            ${lib_dir}/glfw/include
            ${lib_dir}/glm/glm
            ${lib_dir}/stb_image
            ${lib_dir}/assimp/include
            ${lib_dir}/imgui
)

#-------------------------------------------------------------------------
