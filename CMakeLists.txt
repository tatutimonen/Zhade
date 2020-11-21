cmake_minimum_required(VERSION 3.17)

if("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_BINARY_DIR}")
    message(FATAL_ERROR "Only out-of-source builds are supported. \
                         Please use ${CMAKE_SOURCE_DIR}/cmake/InSourceCleanup.cmake \
                         in ${CMAKE_SOURCE_DIR} before attempting to generate project files again.")
endif()

project(shadow-mapper)

set(CMAKE_CXX_STANDARD          17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

#----------------------------------------------------------------------------
# Build types.

#[[
get_property(is_multi_config GLOBAL
    PROPERTY GENERATOR_IS_MULTI_CONFIG
)
if()


if(NOT CMAKE_BUILD_TYPE AND NOT ${is_multi_config})
    message(STATUS "No build type specified, using config 'Release'")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
        STRINGS Debug Release Profile  
    )  
endif()
]]#

#----------------------------------------------------------------------------
# Source files, libraries, etc.

set(libs glew glfw glm)
string(REPLACE ";" "|" libs_pattern "${libs}")

set(libs_dir "${CMAKE_SOURCE_DIR}/lib")
add_subdirectory("${libs_dir}/glew-2.2.0/build/cmake" glew)
add_subdirectory("${libs_dir}/glfw-3.3.2" glfw)
add_subdirectory("${libs_dir}/glm" glm)

add_subdirectory("${CMAKE_SOURCE_DIR}/src" App)

#----------------------------------------------------------------------------
