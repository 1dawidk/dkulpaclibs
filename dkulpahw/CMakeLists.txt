cmake_minimum_required(VERSION 3.12)
project(dkulpahw)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

add_library(dkulpahw bluetooth/BtRFCOMMServer.cpp bluetooth/BtRFCOMMServer.h bluetooth/BluetoothManager.cpp bluetooth/BluetoothManager.h Camera.h Camera.cpp SoundRecorder.cpp SoundRecorder.h)
target_link_libraries(dkulpahw ${OpenCV_LIBS})