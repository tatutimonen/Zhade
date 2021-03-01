add_executable(App
    main.cpp
    Util.cpp
    App.cpp
    Camera.cpp
    Shader.cpp
    ShaderProgram.cpp
)
target_link_libraries(App
    PRIVATE libglew_static
            glfw
            glm
            SOIL
            assimp
)
target_include_directories(App
    PRIVATE "${lib_dir}/glew/include"
            "${lib_dir}/glfw/include"
            "${lib_dir}/glm/glm"
            "${lib_dir}/soil/src"
            "${lib_dir}/assimp/include"
)
