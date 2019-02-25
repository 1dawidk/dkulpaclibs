cmake_minimum_required(VERSION 3.13)
project(catDot)

set(CMAKE_CXX_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(catDot main.cpp Element.cpp Element.h Point.h Vector.cpp Vector.h)
target_link_libraries(catDot GL GLU ${GLFW_LIBRARIES} X11 Xxf86vm Xrandr pthread Xi m dl Xxf86vm Xinerama Xcursor)