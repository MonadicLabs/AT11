cmake_minimum_required(VERSION 2.8)
project(AT11_SERIAL C CXX)

set(CMAKE_CXX_STANDARD 11)

include_directories(${AT11_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

# Serial
include_directories(${PROJECT_SOURCE_DIR}/serial/include)
file (GLOB_RECURSE SERIAL_SRC ./serial/src/*.cc )

# Global shit
file (GLOB AT11_SERIAL_SRC ./*.cc )
file (GLOB AT11_SERIAL_HEADERS ./*.h )
add_executable( at_serial ${AT11_SERIAL_SRC} ${SERIAL_SRC} ${AT11_DUMMY_HEADERS} )

# set( DEPS "${DEPS}" )
target_link_libraries ( at_serial ${DEPS} )
