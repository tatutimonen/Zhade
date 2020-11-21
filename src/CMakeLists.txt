#----------------------------------------------------------------------------
# Executable definition and library linking.

add_executable(App main.cpp App.cpp)
# Ignore the GLEW-related linker warning about unoptimal x64 image base address
if(MSVC)
    set_target_properties(glew
        PROPERTIES
            LINK_FLAGS "/ignore:4281"
    )
endif()
target_link_libraries(App PUBLIC glew glfw glm)
target_include_directories(App
    PUBLIC ${libs_dir}/glew-2.2.0/include
           ${libs_dir}/glfw-3.3.2/include
           ${libs_dir}/glm
)

#----------------------------------------------------------------------------
