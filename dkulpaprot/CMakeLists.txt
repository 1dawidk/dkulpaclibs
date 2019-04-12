cmake_minimum_required(VERSION 3.13)
project(dkulpaprot)

set(CMAKE_CXX_STANDARD 11)

add_library(dkulpaprot defs.h SPI.cpp SPI.h I2C.cpp I2C.h)