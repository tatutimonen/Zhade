add_executable(main
    main.cpp
)
target_link_libraries(main
    PRIVATE libglew_static
            glfw
            glm
)
target_include_directories(main
    PRIVATE "lib/glew-cmake/include"
            "lib/glfw/include"
            "lib/glm/glm"
            "lib/Simple-OpenGL-Image-Library/src"
)