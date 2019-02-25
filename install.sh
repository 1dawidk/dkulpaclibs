#!/bin/bash

# Setup bluez
#
# nano /etc/systemd/system/dbus-org.bluez.service
# ExecStart=/usr/lib/bluetooth/bluetoothd -> ExecStart=/usr/lib/bluetooth/bluetoothd --compat
# sudo systemctl daemon-reload
# sudo systemctl restart bluetooth
# sudo chmod 777 /var/run/sdp

echo "Config blueZ..."
#sed -i -e 's/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd/ExecStart=\/usr\/lib\/bluetooth\/bluetoothd --compat/g'
#systemctl daemon-reload
#systemctl restart bluetooth
chmod 777 /var/run/sdp

#Build misc library
echo "Building dkulpaclibs..."
cd ./dkulpamisc
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpamisc/cmake-build-debug --target all -- -O -j 2
cd ..

cd ./dkulpahw
cmake --build /home/dkulpa/ToolBox/CLionProjects/dkulpaclibs/dkulpahw/cmake-build-debug --target all -- -O -j 2
cd ..

#Create required dirs
echo "Creating necessary dirs..."
mkdir -p /usr/local/include/dkulpaclibs

mkdir -p /usr/local/include/dkulpaclibs/misc

mkdir -p /usr/local/include/dkulpaclibs/hw

#Copy misc include files
echo "Copy include files..."

cd ./dkulpamisc
rsync -av -m --include='*.h' --include='*/' --exclude='*' ./ /usr/local/include/dkulpaclibs/misc/
cd ..

cd ./dkulpahw
rsync -av -m --include='*.h' --include='*/' --exclude='*' ./ /usr/local/include/dkulpaclibs/hw/
cd ..

#Copy find file
echo "Copy cmake Find file..."
cp ./Finddkulpaclibs.cmake /usr/local/lib/cmake/Finddkulpaclibs.cmake
cp ./Finddkulpaclibs.cmake /home/dkulpa/ToolBox/CLion/bin/cmake/linux/share/cmake-3.13/Modules/Finddkulpaclibs.cmake

#Copy misc bin
echo "Copy binary files..."
cp ./dkulpamisc/cmake-build-debug/libdkulpamisc.a /usr/local/lib/libdkulpaclibs_misc.a
cp ./dkulpahw/cmake-build-debug/libdkulpahw.a /usr/local/lib/libdkulpaclibs_hw.a
