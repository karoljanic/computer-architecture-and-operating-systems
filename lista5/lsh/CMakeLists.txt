cmake_minimum_required(VERSION 3.2)

project(lsh)

project(
    lsh
    VERSION 0.1
    LANGUAGES C )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
                -Wall\
                -Wextra\
                -Wpedantic\ ")

file(GLOB_RECURSE SRC src/*.c)

add_executable(main ${SRC})
