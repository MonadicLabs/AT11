cmake_minimum_required(VERSION 2.8)
project (AT11)

option(AT11_BUILD_TESTS "Build Unit tests" OFF)
option(AT11_BUILD_EXAMPLES "Build test apps" ON)

set(CMAKE_CXX_STANDARD 11)

# Find Profiler
file (GLOB_RECURSE AT11_HEADERS ${AT11_SOURCE_DIR}/include/*.h )

if( AT11_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

if( AT11_BUILD_TESTS )
    add_subdirectory( tests )
endif()
