cmake_minimum_required(VERSION 3.7)
project(dkulpahw)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(dkulpaclibs REQUIRED)

add_library(dkulpahw bluetooth/BluetoothManager.cpp bluetooth/BluetoothManager.h Camera.h Camera.cpp SoundRecorder.cpp SoundRecorder.h)
target_link_libraries(dkulpahw ${OpenCV_LIBS} ${dkulpaclibs_misc_LIB})
