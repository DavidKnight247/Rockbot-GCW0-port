#!/bin/sh
VERSIONNAME=`cat version_name.txt`

rm -r -f ./PSP
mkdir ./PSP
mkdir ./PSP/Rockbot
cp ../../EBOOT.PBP ./PSP/Rockbot/
rsync -r --exclude=.svn ../data ./PSP/Rockbot/
rm ./PSP/Rockbot/data/game212.sav
rm ./PSP/Rockbot/data/config_v212.sav
cd ./PSP
zip -r ../Rockbot_PSP_$VERSIONNAME.zip *
