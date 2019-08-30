cmake_minimum_required(VERSION 2.8)
project(AT11_DUMMY C CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${AT11_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

# Global shit
file (GLOB_RECURSE AT11_DUMMY_SRC ./*.cc )
file (GLOB_RECURSE AT11_DUMMY_HEADERS ./*.h )
add_executable( dummy ${AT11_DUMMY_SRC} ${AT11_DUMMY_HEADERS} )

# set( DEPS "${DEPS}" )
target_link_libraries ( dummy ${DEPS} )
