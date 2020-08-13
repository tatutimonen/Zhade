cmake_minimum_required(VERSION 3.17)
project(shadow-mapper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App main.cpp)

# Dependencies
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
target_include_directories(App
    PUBLIC ${DEPS_DIR}/glew-2.1.0/include/
           ${DEPS_DIR}/glfw-3.3.2/include/
           ${DEPS_DIR}/glm/
)
