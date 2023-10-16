cmake_minimum_required(VERSION 3.7)

project(BrickOut)

file (GLOB Foo_SOURCES CONFIGURE_DEPENDS "*.c")
file (GLOB Foo_HEADERS CONFIGURE_DEPENDS "*.h")

set (Foo_INCLUDE_DIRS "")
foreach (_headerFile ${Foo_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND Foo_INCLUDE_DIRS ${_dir})
endforeach()

list (REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(BrickOut ${Foo_SOURCES})
target_link_libraries(BrickOut ${SDL2_LIBRARIES})