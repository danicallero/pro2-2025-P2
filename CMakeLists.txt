#PLEASE, DO NOT CHANGE THIS FILE
cmake_minimum_required(VERSION 3.5)
project(P2 C)
set(CMAKE_C_STANDARD 99)

include(config.cmake)

set(CMAKE_VERBOSE_MAKEFILE true)
set(warnings -Wall -Wextra)

# Compilar bid_stack como librería estática
add_library(bid_stack STATIC bid_stack.c bid_stack.h)

# Compilar console_list como librería estática
add_library(console_list STATIC console_list.c console_list.h)

# main: necesita main.c, types.h y ambas librerías
add_definitions(-DINPUT_FILE="${input}")
add_executable(main main.c types.h)
target_compile_options(main PRIVATE ${warnings})
target_link_libraries(main console_list bid_stack)

# Añadimos el subdirectorio adt_test si tienes tests extra allí
add_subdirectory(adt_test)

# Salida de binarios en ../bin
set_target_properties(main test_console_list test_bid_stack PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
