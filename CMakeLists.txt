cmake_minimum_required(VERSION 3.17)
project(shadow-mapper)

# Dependencies
set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)
add_subdirectory(${DEPS_DIR}/glew-2.2.0/build/cmake/ glew)
add_subdirectory(${DEPS_DIR}/glfw-3.3.2/ glfw)
add_subdirectory(${DEPS_DIR}/glm/ glm)

# Project source files
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ App)
