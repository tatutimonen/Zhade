cmake_minimum_required(VERSION 3.17)
project(shadow-mapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App main.cpp App.cpp)

# CMake actually builds all of these in the background if necessary
target_link_libraries(App
    PUBLIC glew glfw glm
)

# Dependencies
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(App
    PUBLIC ${DEPS_DIR}/glew-2.2.0/include/
           ${DEPS_DIR}/glfw-3.3.2/include/
           ${DEPS_DIR}/glm/
)
