set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App main.cpp App.cpp)
# Ignore the linker warning about unoptimal x64 image base address (Windows,)
set_target_properties(glew PROPERTIES LINK_FLAGS "/ignore:4281")
# CMake actually builds all of these in the background if necessary
target_link_libraries(App PUBLIC glew glfw glm)
# Dependencies
set(DEP_DIR ${CMAKE_SOURCE_DIR}/dep)
target_include_directories(App
    PUBLIC ${DEP_DIR}/glew-2.2.0/include/
           ${DEP_DIR}/glfw-3.3.2/include/
           ${DEP_DIR}/glm/
)