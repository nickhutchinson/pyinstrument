cmake_minimum_required(VERSION 3.0)
project(pyinstrument CXX)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/third_party/cotire/CMake")
include (cotire)

if (CMAKE_SYSTEM_NAME MATCHES Darwin)
    add_compile_options (-stdlib=libc++)
    link_libraries (-stdlib=libc++)
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-Wall -Wextra -Wconversion)
    add_compile_options(-std=c++11 -fvisibility=hidden)
endif ()

add_subdirectory (third_party)
add_subdirectory (pyinstrument)

