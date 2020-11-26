add_executable(main
    main.cpp
)
target_link_libraries(main
    PRIVATE libglew_static
            glfw
            glm
            SOIL
)
target_include_directories(main
    PRIVATE "${lib_dir}/glew/include"
            "${lib_dir}/glfw/include"
            "${lib_dir}/glm/glm"
            "${lib_dir}/soil/src"
)