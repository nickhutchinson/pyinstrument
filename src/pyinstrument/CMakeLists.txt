include( cotire )

include_directories( .. )

add_library( pyinstrument SHARED
    pyinstrument_prefix.h
    frame.cc
    frame.h
    profiler.cc
    profiler.h
    python_module.cc
    string_pool.h
)

target_include_directories( pyinstrument PRIVATE .. )
target_link_libraries( pyinstrument PRIVATE ThirdParty_Python
    ThirdParty_MiniBoost)

set_target_properties( pyinstrument PROPERTIES
    COTIRE_CXX_PREFIX_HEADER_INIT pyinstrument_prefix.h
    OUTPUT_NAME _pyinstrument_cmodule
    LIBRARY_OUTPUT_DIRECTORY ../
    PREFIX ""
    SUFFIX ".so"
)
cotire( pyinstrument )

