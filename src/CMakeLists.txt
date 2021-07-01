add_executable(App
    App.cpp
    Camera.cpp
    Common.cpp
    ITexture.cpp
    main.cpp
    Mesh.cpp
    Shader.cpp
    ShaderProgram.cpp
    Texture2D.cpp
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
