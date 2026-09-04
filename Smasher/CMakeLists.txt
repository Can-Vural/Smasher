cmake_minimum_required(VERSION 3.20)
project(Smasher C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(Smasher
        src/main.c
        src/huffman.c
        src/bit_io.c
        src/compress.c
)

add_custom_target(link_data ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
)
add_dependencies(Smasher link_data)