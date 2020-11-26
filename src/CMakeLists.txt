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
    PRIVATE "lib/glew/include"
            "lib/glfw/include"
            "lib/glm/glm"
            "lib/soil/src"
)