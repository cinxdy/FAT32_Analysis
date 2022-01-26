cmake_minimum_required (VERSION 3.16)

project (develop)

set(SOURCES main.cpp)
set (CMAKE_VERBOSE_MAKEFILE true)

include_directories (include)
add_subdirectory(src)
link_libraries(core)
add_executable(app.out ${SOURCES})