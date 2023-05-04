# A simple script that removes files and directories created by running `cmake .` in the project directory.

file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/CMakeFiles")
file(REMOVE "${CMAKE_SOURCE_DIR}/CMakeCache.txt")
