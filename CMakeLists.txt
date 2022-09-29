cmake_minimum_required(VERSION 3.17)

#-------------------------------------------------------------------------
# Sanity checks.

if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    message(FATAL_ERROR
        "Only out-of-source builds are supported. \
         Please use ${CMAKE_SOURCE_DIR}/cmake/InSourceCleanup.cmake \
         in ${CMAKE_SOURCE_DIR} before attempting to generate project files again."
    )
endif()

#-------------------------------------------------------------------------
# Toolchain, project definition, modules, packages.

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file"
)

project(Zhade CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#-------------------------------------------------------------------------
# Libraries.

find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
find_package(imgui CONFIG REQUIRED)
find_package(robin_hood CONFIG REQUIRED)

#-------------------------------------------------------------------------

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${PROJECT_NAME})

#-------------------------------------------------------------------------
