#!/bin/sh
VERSIONNAME=`cat version_name.txt`

rm -r -f ./Dingux
mkdir ./Dingux
mkdir ./Dingux/Rockbot
cp ../rockbot.dge ./Dingux/Rockbot/
rsync -r --exclude=.svn ../data ./Dingux/Rockbot/
rm ./Dingux/Rockbot/data/game212.sav
rm ./Dingux/Rockbot/data/config_v212.sav

cp ./Dingux/Rockbot/data/images/faces/rockbot.png ./Dingux/Rockbot/
cd ./Dingux
zip -r ../Rockbot_Dingux_$VERSIONNAME.zip *
