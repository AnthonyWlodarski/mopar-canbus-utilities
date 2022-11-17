#!/bin/bash

rm -rf mopar_canbus_utils.deb
rm -rf mopar_canbus_utils
mkdir -p mopar_canbus_utils/DEBIAN

cat > ./mopar_canbus_utils/DEBIAN/control<< EOF
Package: mopar-canbus-utils
Version: 1.0
Section: custom
Priority: optional
Architecture: all
Essential: no
Installed-Size: 1024
Maintainer: Anthony W.
Description: Canbus utilities to use with Mopar GPEC based controllers.
EOF

mkdir -p mopar_canbus_utils/opt/mopar_canbus_utils

for dir in ./*; do
	if test -f "$dir/makefile"; then
		cd $dir;
		make clean && make
	       	cd -
		cp "$dir/$dir" mopar_canbus_utils/opt/mopar_canbus_utils	
	fi
done


sudo dpkg-deb --build ./mopar_canbus_utils
sudo dpkg -I ./mopar_canbus_utils.deb
sudo dpkg -i ./mopar_canbus_utils.deb
