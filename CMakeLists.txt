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
# Project definition, modules, packages.

project(shadow-mapper CXX)

list(APPEND CMAKE_MODULE_PATH "{CMAKE_SOURCE_DIR}/cmake")

find_package(OpenMP REQUIRED)

#-------------------------------------------------------------------------
# Libraries.

set(lib_dir ${CMAKE_SOURCE_DIR}/lib)

add_library(stb_image STATIC ${lib_dir}/stb_image/stb_image.cpp)

set(imgui_src_dir ${lib_dir}/imgui/backends)
add_library(imgui STATIC ${imgui_src_dir}/imgui_impl_win32.cpp ${imgui_src_dir}/imgui_impl_opengl3.cpp)
target_include_directories(imgui PRIVATE ${lib_dir}/imgui)

add_compile_definitions(GLEW_STATIC)
add_subdirectory(${lib_dir}/glew EXCLUDE_FROM_ALL)
add_subdirectory(${lib_dir}/glfw EXCLUDE_FROM_ALL)
add_subdirectory(${lib_dir}/glm EXCLUDE_FROM_ALL)
add_subdirectory(${lib_dir}/assimp EXCLUDE_FROM_ALL)

#-------------------------------------------------------------------------

add_subdirectory(${CMAKE_SOURCE_DIR}/src App)

#-------------------------------------------------------------------------
