cmake_minimum_required(VERSION 3.17)
project("shadow-mapper")

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Only out-of-source builds are supported.")
endif()

#----------------------------------------------------------------------------
# Subdirectories.

set(EXTERN_DIR "${CMAKE_SOURCE_DIR}/extern")
add_subdirectory("${EXTERN_DIR}/glew-2.2.0/build/cmake" glew)
add_subdirectory("${EXTERN_DIR}/glfw-3.3.2" glfw)
add_subdirectory("${EXTERN_DIR}/glm" glm)

add_subdirectory("${CMAKE_SOURCE_DIR}/src" App)

#----------------------------------------------------------------------------
# Cleanup.

if("${CLEANUP_MODE}" STREQUAL "")
    set(CLEANUP_MODE "simple")
endif()
add_custom_target(Cleanup
    COMMAND "${CMAKE_COMMAND}"
            -D "CLEANUP_MODE=${CLEANUP_MODE}"
            -P "${CMAKE_SOURCE_DIR}/cmake/cleanup.cmake"
)

#----------------------------------------------------------------------------
