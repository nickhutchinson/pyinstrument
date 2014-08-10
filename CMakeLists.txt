cmake_minimum_required( VERSION 2.8.12 )
project( pyinstrument CXX )

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug FORCE CACHE STRING "" )
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_MACOSX_RPATH ON )

add_subdirectory(pyinstrument)
