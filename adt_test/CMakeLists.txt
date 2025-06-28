cmake_minimum_required(VERSION 3.5)
project(ADT_Test C)
set(CMAKE_C_STANDARD 99)

set(CMAKE_VERBOSE_MAKEFILE true)
set(warnings -Wall -Wextra)

# Incluir directorio padre para los headers bid_stack.h, console_list.h, etc.
include_directories(..)

# Archivos de la librería en directorio padre
set(dependencies_stack ../bid_stack.h ../bid_stack.c)
set(dependencies_list ../console_list.h ../console_list.c)

# Ejecutable test para bid_stack
add_executable(test_bid_stack ../types.h ${dependencies_stack} test_bid_stack.c)
target_compile_options(test_bid_stack PRIVATE ${warnings})

# Ejecutable test para console_list
add_executable(test_console_list ../types.h ${dependencies_list} test_console_list.c)
target_compile_options(test_console_list PRIVATE ${warnings})

# Directorio para salida de binarios
set_target_properties(test_bid_stack test_console_list PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
