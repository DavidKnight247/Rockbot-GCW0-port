#!/bin/sh

rm -r -f ./linux
mkdir ./linux
mkdir ./linux/rockbot
cp ./docs/README.LINUX.TXT ./linux/README.TXT
cp ../rockbot ./linux/rockbot
cp ../editor ./linux/rockbot
rsync -r --exclude=.svn ../data ./linux/rockbot
cd ./linux
tar -cvf ../Rockbot_Linux_1.0b1.tar rockbot
gzip ../Rockbot_Linux.tar
