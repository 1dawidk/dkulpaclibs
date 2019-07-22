cmake_minimum_required(VERSION 3.13)
project(dkulpadev)

set(CMAKE_CXX_STANDARD 11)

add_library(dkulpadev DHT22.cpp DHT22.h HD44780.cpp HD44780.h)