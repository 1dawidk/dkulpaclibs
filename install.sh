#!/bin/bash

#Build misc library
echo "Building dkulpaclibs..."
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpamisc/cmake-build-debug --target all -- -O -j 2
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpahw/cmake-build-debug --target all -- -O -j 2

#Create required dirs
echo "Creating necessary dirs..."
mkdir -p /usr/local/include/dkulpaclibs

mkdir -p /usr/local/include/dkulpaclibs/misc
mkdir -p /usr/local/include/dkulpaclibs/misc/debug

mkdir -p /usr/local/include/dkulpaclibs/hw
mkdir -p /usr/local/include/dkulpaclibs/hw/bluetooth

#Copy misc include files
echo "Copy include files..."

cp ./dkulpamisc/Clock.h /usr/local/include/dkulpaclibs/misc/
cp ./dkulpamisc/Thread.h /usr/local/include/dkulpaclibs/misc/
cp ./dkulpamisc/ConfigManager.h /usr/local/include/dkulpaclibs/misc
cp ./dkulpamisc/debug/Log.h /usr/local/include/dkulpaclibs/misc/debug/

cp ./dkulpahw/Camera.h /usr/local/include/dkulpaclibs/hw/
cp ./dkulpahw/SoundRecorder.h /usr/local/include/dkulpaclibs/hw/
cp ./dkulpahw/bluetooth/BtRFCOMMServer.h /usr/local/include/dkulpaclibs/hw/bluetooth
cp ./dkulpahw/bluetooth/BluetoothManager.h /usr/local/include/dkulpaclibs/hw/bluetooth

#Copy find file
echo "Copy cmake Find file..."
cp ./Finddkulpaclibs.cmake /usr/local/lib/cmake/Finddkulpaclibs.cmake

#Copy misc bin
echo "Copy binary files..."
cp ./dkulpamisc/cmake-build-debug/libdkulpamisc.a /usr/local/lib/libdkulpaclibs_misc.a
cp ./dkulpahw/cmake-build-debug/libdkulpahw.a /usr/local/lib/libdkulpaclibs_hw.a
