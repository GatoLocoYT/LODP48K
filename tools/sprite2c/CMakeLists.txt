cmake_minimum_required(VERSION 3.20)

project(sprite2c C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(sprite2c
    main.c
    ../common/bmp.c
    ../common/writer.c
)

target_include_directories(sprite2c PRIVATE
    ../common
)