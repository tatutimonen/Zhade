cmake_minimum_required(VERSION 3.17)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Only out-of-source builds are supported. \
                         Please use ${CMAKE_SOURCE_DIR}/cmake/InSourceCleanup.cmake \
                         in ${CMAKE_SOURCE_DIR} before attempting to generate project files again.")
endif()

project("shadow-mapper")

set(CMAKE_MODULE_PATH "{CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(libs_dir "${CMAKE_SOURCE_DIR}/lib")
set(GLEW_DIR "${libs_dir}/glew-2.2.0/build/cmake")
find_package(GLEW REQUIRED)
option(GLFW_BUILD_DOCS     OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS    OFF)
add_subdirectory("${libs_dir}/glfw-3.3.2" glfw)
add_subdirectory("${libs_dir}/glm" glm)

add_subdirectory("${CMAKE_SOURCE_DIR}/src" App)
