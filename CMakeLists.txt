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
# Toolchain, project definition, includes, etc.

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file"
)
set(VCPKG_BUILD_TYPE release)

project(Zhade CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    if(NOT CMAKE_VS_GLOBALS MATCHES "(^|;)UseMultiToolTask=")
        list(APPEND CMAKE_VS_GLOBALS UseMultiToolTask=true)
    endif()
    if(NOT CMAKE_VS_GLOBALS MATCHES "(^|;)EnforceProcessCountAcrossBuilds=")
        list(APPEND CMAKE_VS_GLOBALS EnforceProcessCountAcrossBuilds=true)
    endif()
endif()

#-------------------------------------------------------------------------
# Libraries.

find_package(absl CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(robin_hood CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

#-------------------------------------------------------------------------

add_subdirectory(${CMAKE_SOURCE_DIR}/src ${PROJECT_NAME})

#-------------------------------------------------------------------------
