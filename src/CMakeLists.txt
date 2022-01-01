#-------------------------------------------------------------------------
# Source files.

set(src_files
    AmbientLight.cpp
    App.cpp
    Camera.cpp
    DirectionalLight.cpp
    Framebuffer.cpp
    Mesh.cpp
    MeshFactory.cpp
    Observer.cpp
    OrthographicCamera.cpp
    PerspectiveCamera.cpp
    PointLight.cpp
    Shader.cpp
    ShaderProgram.cpp
    Subject.cpp
    Texture.cpp
    Texture2D.cpp
    UniformBuffer.cpp
    UniformBufferStorage.cpp
    common.cpp
    constants.cpp
    main.cpp
    util.cpp
)

#-------------------------------------------------------------------------
# Executable definition, library linking, etc.

add_executable(App ${src_files})

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_link_libraries(App
    PRIVATE libglew_static
            glfw
            glm
            stb_image
)
target_include_directories(App
    PRIVATE ${lib_dir}/glew/include
            ${lib_dir}/glfw/include
            ${lib_dir}/glm/glm
            ${lib_dir}/stb_image
)

#-------------------------------------------------------------------------
