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



set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic")

# Add cppcheck target
find_program(CPPCHECK cppcheck)
if (CPPCHECK)
    set(CPPCHECK_ARGS "--enable=all --std=c18 --language=c --verbose ${Foo_INCLUDE_DIRS} ${Foo_SOURCES}")
    add_custom_target(cppcheck COMMAND ${CPPCHECK} ${CPPCHECK_ARGS})
else()
    message(WARNING "cppcheck not found, skipping static code analysis.")
endif()


INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf)

add_executable(BrickOut ${Foo_SOURCES})


INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)
