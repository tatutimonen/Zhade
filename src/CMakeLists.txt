add_executable(App main.cpp)

target_link_libraries(App
    PUBLIC glfw ${GLFW_LIBRARIES}
           glm
)
target_include_directories(App
    PUBLIC "${GLEW_INCLUDE_DIRS}"
           "${libs_dir}/glfw-3.3.2/include"
           "${libs_dir}/glm"
)