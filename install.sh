#!/bin/bash

# Setup bluez
#
# nano /etc/systemd/system/dbus-org.bluez.service
# ExecStart=/usr/lib/bluetooth/bluetoothd -> ExecStart=/usr/lib/bluetooth/bluetoothd --compat
# sudo systemctl daemon-reload
# sudo systemctl restart bluetooth
# sudo chmod 777 /var/run/sdp

echo "Install required dependencies"
apt install libbluetooth-dev libasound2-dev libcurl4-openssl-dev libopencv-dev

echo "Config blueZ..."
#sed -i -e 's/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd --compat/g'
#systemctl daemon-reload
#systemctl restart bluetooth
chmod 777 /var/run/sdp

#Create required dirs
echo "Creating necessary dirs..."
mkdir -p /usr/local/include/dkulpaclibs
mkdir -p /usr/local/include/dkulpaclibs/misc
mkdir -p /usr/local/include/dkulpaclibs/hw

#Copy find file
echo "Copy cmake Find file..."
cp ./Finddkulpaclibs.cmake /usr/local/lib/cmake/Finddkulpaclibs.cmake
cp ./Finddkulpaclibs.cmake /usr/share/cmake-3.7/Modules/Finddkulpaclibs.cmake
cp ./Finddkulpaclibs.cmake /home/dkulpa/ToolBox/CLion/bin/cmake/linux/share/cmake-3.13/Modules/Finddkulpaclibs.cmake

#Build misc library
echo "Building dkulpaclibs misc..."
mkdir ./dkulpamisc/build
cd ./dkulpamisc/build
cmake ..
make
cd ..

rsync -av -m --include='*.h' --include='*/' --exclude='*' ./ /usr/local/include/dkulpaclibs/misc/
cd ..

#Build hw library
echo "Building dkulpaclibs hw..."
mkdir ./dkulpahw/build
cd ./dkulpahw/build
cmake ..
make
cd ..

rsync -av -m --include='*.h' --include='*/' --exclude='*' ./ /usr/local/include/dkulpaclibs/hw/
cd ..

#Copy misc bin
echo "Copy binary files..."
cp ./dkulpamisc/build/libdkulpamisc.a /usr/local/lib/libdkulpaclibs_misc.a
cp ./dkulpahw/build/libdkulpahw.a /usr/local/lib/libdkulpaclibs_hw.a
