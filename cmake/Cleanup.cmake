
set(libs "glew" "glfw" "glm")
string(REPLACE ";" "|" libs_pattern "${libs}")
message("${libs_pattern}")
target
#[[file(GLOB files "*")
if(NOT ${CLEAN_ALL})
    list(FILTER files EXCLUDE REGEX "glew|glfw|glm")
endif()
foreach(f ${files})
    message("Removing ${f}")
    #file(REMOVE_RECURSE "${f}")
endforeach()]]
