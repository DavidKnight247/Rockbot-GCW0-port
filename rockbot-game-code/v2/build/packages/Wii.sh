#!/bin/sh

VERSIONNAME=`cat version_name.txt`

rm -r -f ./Wii
mkdir ./Wii
mkdir ./Wii/apps
mkdir ./Wii/apps/rockbot
cp ../rockbot.elf ./Wii/apps/rockbot
rsync -r --exclude=.svn ../data ./Wii/apps/rockbot
rm ./Wii/apps/rockbot/data/game212.sav
rm ./Wii/apps/rockbot/data/config_v212.sav

cd ./Wii
zip -r ../Rockbot_Wii_$VERSIONNAME.zip *
