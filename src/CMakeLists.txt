set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App main.cpp App.cpp)
# Ignore the GLEW-related linker warning about unoptimal x64 image base address
if(WIN32)
    set_target_properties(glew PROPERTIES LINK_FLAGS "/ignore:4281")
endif()
target_link_libraries(App PUBLIC glew glfw glm)
target_include_directories(App
    PUBLIC ${EXTERN_DIR}/glew-2.2.0/include
           ${EXTERN_DIR}/glfw-3.3.2/include
           ${EXTERN_DIR}/glm
)
