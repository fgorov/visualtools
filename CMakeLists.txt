cmake_minimum_required(VERSION 3.20)

project(visualtools LANGUAGES CXX)

function(print)
    foreach(var ${ARGN})
        message("${var} = ${${var}}")
    endforeach()
endfunction()

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/gradient.cpp
    src/display.cpp
    src/coords.cpp
    src/shapes.cpp
)