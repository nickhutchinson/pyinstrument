find_package(PythonLibs REQUIRED )
add_library(ThirdParty_Python INTERFACE)

target_link_libraries(ThirdParty_Python INTERFACE ${PYTHON_LIBRARIES})
target_include_directories(ThirdParty_Python SYSTEM INTERFACE
    ${PYTHON_INCLUDE_DIRS}
)

# ------------------------------------------------------------------------------

file(GLOB_RECURSE src boost/*.cpp)
file(GLOB_RECURSE to_remove boost/libs/thread/src/win32/*)
if (to_remove) 
    list(REMOVE_ITEM src ${to_remove})
endif ()

add_library(ThirdParty_MiniBoost STATIC ${src})

target_include_directories(ThirdParty_MiniBoost 
    SYSTEM INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/boost")
target_include_directories(ThirdParty_MiniBoost 
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/boost")

set_property(TARGET ThirdParty_MiniBoost PROPERTY OUTPUT_NAME boost)
target_compile_options(ThirdParty_MiniBoost PRIVATE -w)
target_link_libraries(ThirdParty_MiniBoost PUBLIC ThirdParty_Python)

