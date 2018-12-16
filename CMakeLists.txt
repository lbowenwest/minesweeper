cmake_minimum_required(VERSION 3.12)
project(minesweeper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

file(GLOB SOURCE_FILES src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES src/main.cpp)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

pkg_check_modules(SFML REQUIRED sfml-graphics)
link_directories(${SFML_LIBRARY_DIRS})
include_directories(${SFML_INCLUDE_DIRS})

add_executable(minesweeper src/main.cpp ${SOURCE_FILES})
target_link_libraries(minesweeper ${GTKMM_LIBRARIES} ${SFML_LIBRARIES})
