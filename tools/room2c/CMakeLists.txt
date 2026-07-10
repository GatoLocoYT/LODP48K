cmake_minimum_required(VERSION 3.20)

project(room2c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(room2c
    main.c
    ../common/bmp.c
)

target_include_directories(room2c PRIVATE
    ../common
)