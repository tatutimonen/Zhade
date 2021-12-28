add_executable(App
    AmbientLight.cpp
    App.cpp
    Camera.cpp
    Common.cpp
    Constants.cpp
    DirectionalLight.cpp
    main.cpp
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
    Util.cpp
)
target_link_libraries(App
    PRIVATE libglew_static
            glfw
            glm
            SOIL
#            assimp
)
target_include_directories(App
    PRIVATE "${lib_dir}/glew/include"
            "${lib_dir}/glfw/include"
            "${lib_dir}/glm/glm"
            "${lib_dir}/soil/src"
#           "${lib_dir}/assimp/include"
)
