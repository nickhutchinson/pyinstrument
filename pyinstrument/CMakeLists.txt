include_directories( .. )

find_package( Boost REQUIRED COMPONENTS Python )
find_package( PythonLibs REQUIRED )

add_library( pyinstrument SHARED
    frame.cc
    frame.h
    python_profiler.cc
    string_pool.h
)

target_link_libraries(pyinstrument
    ${PYTHON_LIBRARIES}
    ${Boost_LIBRARIES}
)

target_include_directories(pyinstrument PRIVATE
    ${PYTHON_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

if( CMAKE_PLATFORM_NAME MATCHES Darwin )
    target_compile_options( pyinstrument PRIVATE -stdlib=libc++ )
    target_link_libraries( pyinstrument PRIVATE -stdlib=libc++ )
endif()


target_compile_options( pyinstrument PRIVATE
    -Wall
    -Wextra
    -std=c++11
)

