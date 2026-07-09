cmake_minimum_required(VERSION 3.20)

project(LODP48K VERSION 0.1.0 LANGUAGES C)

# ===========================
# Configuración del compilador
# ===========================

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ===========================
# Buscar SDL2
# ===========================

find_package(SDL2 REQUIRED)

# ===========================
# Ejecutable
# ===========================

add_executable(LODP48K
    src/main.c
    src/game.c
    src/renderer.c
)

# ===========================
# Includes
# ===========================

target_include_directories(LODP48K PRIVATE
    src
)

# ===========================
# Librerías
# ===========================

target_link_libraries(LODP48K PRIVATE
    SDL2::SDL2
)