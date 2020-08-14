cmake_minimum_required(VERSION 3.17)
project(shadow-mapper)

# Dependencies
set(EXTERN_DIR ${CMAKE_SOURCE_DIR}/extern)
add_subdirectory(${EXTERN_DIR}/glew-2.2.0/build/cmake/ glew)
add_subdirectory(${EXTERN_DIR}/glfw-3.3.2/ glfw)
add_subdirectory(${EXTERN_DIR}/glm/ glm)

# Project source files
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ App)
