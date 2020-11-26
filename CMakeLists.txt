cmake_minimum_required(VERSION 3.17)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Only out-of-source builds are supported. \
                         Please use ${CMAKE_SOURCE_DIR}/cmake/InSourceCleanup.cmake \
                         in ${CMAKE_SOURCE_DIR} before attempting to generate project files again.")
endif()

project(shadow-mapper)

set(CMAKE_MODULE_PATH "{CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

find_package(OpenMP REQUIRED)

set(lib_dir "${CMAKE_SOURCE_DIR}/lib")
add_compile_definitions(GLEW_STATIC)
add_subdirectory("${lib_dir}/glew" EXCLUDE_FROM_ALL)
add_subdirectory("${lib_dir}/glfw" EXCLUDE_FROM_ALL)
add_subdirectory("${lib_dir}/glm"  EXCLUDE_FROM_ALL)
add_subdirectory("${lib_dir}/soil" EXCLUDE_FROM_ALL)

add_subdirectory("${CMAKE_SOURCE_DIR}/src" main)
