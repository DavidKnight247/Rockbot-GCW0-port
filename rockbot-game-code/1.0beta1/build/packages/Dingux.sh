#!/bin/sh

rm -r -f ./Dingux
mkdir ./Dingux
mkdir ./Dingux/Rockbot
cp ../rockbot.dge ./Dingux/Rockbot/
rsync -r --exclude=.svn ../data ./Dingux/Rockbot/
cd ./Dingux
zip -r ../Rockbot_Dingux_1.0a1.zip *
