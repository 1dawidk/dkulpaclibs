cmake_minimum_required(VERSION 3.7)
project(dkulpamisc)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_library(dkulpamisc debug/Log.cpp debug/Log.h Clock.cpp Clock.h Thread.h Thread.cpp ConfigManager.cpp ConfigManager.h PostPairs.cpp PostPairs.h)
target_link_libraries(dkulpamisc ${CMAKE_THREAD_LIBS_INIT})
