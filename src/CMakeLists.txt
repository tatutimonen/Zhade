cmake_minimum_required(VERSION 3.17)

project(shadow-mapper LANGUAGES CXX)

find_package(glew REQUIRED)
find_package(glfw3 REQUIRED) # doesn't seem to be logged even if found?
find_package(glm REQUIRED) # neither this one

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp)

# FindGLM.cmake doesn't define a target to link against. This workaround is from here:
# https://github.com/bincrafters/community/issues/733
target_link_libraries(${PROJECT_NAME} glfw ${GLM_INCLUDE_DIR}) 