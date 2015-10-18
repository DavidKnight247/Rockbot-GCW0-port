#!/bin/sh

###GCWZERO BUILD SCRIPT###

#This should be run from the build/packages folder!

VERSIONNAME=`cat version_name.txt`

#make executable
cd ../..
make clean
qmake CONFIG+=gcwzero
env GCWZERO_TOOLCHAIN_PREFIX=/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot/usr/ make
cd build/packages

#create build folder
rm -r -f ./GCWZero
mkdir ./GCWZero
mkdir ./GCWZero/Rockbot
cp ../../rockbot ./GCWZero/Rockbot/
rsync -r --exclude=.svn ../data ./GCWZero/Rockbot/
rm ./GCWZero/Rockbot/data/game*.sav
rm ./GCWZero/Rockbot/data/config_*.sav

cp ./GCWZero/Rockbot/data/images/faces/rockbot.png ./GCWZero/Rockbot/

cd ./GCWZero

# create desktop file
echo "\
[Desktop Entry]

Type=Application
Name=Rockbot $VERSIONNAME
Comment=A homage to Mega Man
Exec=rockbot
Icon=rockbot
Categories=games;
X-OD-Manual=readme.txt
" > Rockbot/default.gcw0.desktop

# create opk
mksquashfs Rockbot Rockbot$VERSIONNAME.opk -all-root -noappend -no-exports -no-xattrs
