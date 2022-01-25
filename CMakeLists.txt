cmake_minimum_required (VERSION 3.16)

project (develop)

set (CMAKE_CXX_STANDARD 23)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES main.cpp)

include_directories ( 
  include
)
add_subdirectory(src)