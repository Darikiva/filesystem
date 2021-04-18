cmake_minimum_required(VERSION 3.0.0)
project(wow VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)

set(TOP_DIR ${CMAKE_SOURCE_DIR})
set(SRC_DIR ${TOP_DIR}/src)
set(INC_DIR ${TOP_DIR}/include)

include_directories(${INC_DIR})
include_directories(${SRC_DIR})

file(GLOB_RECURSE FILESYSTEM_SRC *.cpp)

add_library(Wow STATIC ${FILESYSTEM_SRC})
set_target_properties(Wow PROPERTIES LINKER_LANGUAGE CXX)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE Wow)