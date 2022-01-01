add_executable(App
    AmbientLight.cpp
    App.cpp
    Camera.cpp
    DirectionalLight.cpp
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
target_link_libraries(App
    PRIVATE libglew_static
            glfw
            glm
            stb_image
#            assimp
)
target_include_directories(App
    PRIVATE ${lib_dir}/glew/include
            ${lib_dir}/glfw/include
            ${lib_dir}/glm/glm
            ${lib_dir}/stb_image
#           "${lib_dir}/assimp/include"
)
