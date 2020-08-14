cmake_minimum_required(VERSION 3.17)
project(shadow-mapper)

# Dependencies
set(DEP_DIR ${CMAKE_SOURCE_DIR}/dep)
add_subdirectory(${DEP_DIR}/glew-2.2.0/build/cmake/ glew)
add_subdirectory(${DEP_DIR}/glfw-3.3.2/ glfw)
add_subdirectory(${DEP_DIR}/glm/ glm)

# Project source files
add_subdirectory(${CMAKE_SOURCE_DIR}/src/ App)
