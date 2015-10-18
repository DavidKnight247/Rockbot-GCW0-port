#!/bin/sh

cd ../game
make clean
make -f Makefile.ps2 clean
make -f Makefile.nds clean
make -f Makefile.dingux clean
make -f Makefile.dingux

cd ../packages
rm -r -f ./Dingux
mkdir ./Dingux
mkdir ./Dingux/Rockbot
cp ../bin/rockbot.dge ./Dingux/Rockbot/
sh ../bin/pack_data.sh
cp -r ./data ./Dingux/Rockbot
cd ./Dingux
zip -r ../Rockbot_Dingux.zip *
