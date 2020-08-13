cmake_minimum_required(VERSION 3.17)

project(shadow-mapper LANGUAGES CXX)

find_library()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(App main.cpp)

# FindGLM.cmake doesn't define a target to link against. This workaround is from here:
# https://github.com/bincrafters/community/issues/733
target_link_libraries(App 
    PUBLIC glfw ${GLM_INCLUDE_DIR}
)
