#!/bin/bash

#Build misc library
echo "Building dkulpaclibs..."
cd ./dkulpamisc
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpamisc/cmake-build-debug --target all -- -O -j 2
cd ./../dkulpahw
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpahw/cmake-build-debug --target all -- -O -j 2
cd ..

#Create required dirs
echo "Creating necessary dirs..."
mkdir -p /usr/local/include/dkulpaclibs

mkdir -p /usr/local/include/dkulpaclibs/misc
mkdir -p /usr/local/include/dkulpaclibs/misc/debug

mkdir -p /usr/local/include/dkulpaclibs/hw
mkdir -p /usr/local/include/dkulpaclibs/hw/bluetooth

#Copy misc include files
echo "Copy include files..."

cd ./dkulpamisc
find . -type f | grep -i .h$ | xargs -i cp {} /usr/local/include/dkulpaclibs/misc/{}
cd ..

cd ./dkulpahw
find . -type f | grep -i .h$ | xargs -i cp {} /usr/local/include/dkulpaclibs/hw/{}
cd ..

#Copy find file
echo "Copy cmake Find file..."
cp ./Finddkulpaclibs.cmake /usr/local/lib/cmake/Finddkulpaclibs.cmake
cp ./Finddkulpaclibs.cmake /home/dkulpa/ToolBox/CLion/bin/cmake/linux/share/cmake-3.13/Modules/Finddkulpaclibs.cmake

#Copy misc bin
echo "Copy binary files..."
cp ./dkulpamisc/cmake-build-debug/libdkulpamisc.a /usr/local/lib/libdkulpaclibs_misc.a
cp ./dkulpahw/cmake-build-debug/libdkulpahw.a /usr/local/lib/libdkulpaclibs_hw.a
