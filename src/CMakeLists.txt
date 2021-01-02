add_executable(main
    main.cpp
    gl_common.hpp
    App.cpp
    Camera.cpp
    Shader.cpp
    ShaderProgram.cpp
)
target_link_libraries(main
    PRIVATE libglew_static
            glfw
            glm
            SOIL
            assimp
)
target_include_directories(main
    PRIVATE "${lib_dir}/glew/include"
            "${lib_dir}/glfw/include"
            "${lib_dir}/glm/glm"
            "${lib_dir}/soil/src"
            "${lib_dir}/assimp/include"
)
