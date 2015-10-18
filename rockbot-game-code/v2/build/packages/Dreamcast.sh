#!/bin/sh
VERSIONNAME=`cat version_name.txt`

rm -r -f ./DC/rockbot
mkdir ./DC/rockbot
cp ./docs/README.LINUX.TXT ./DC/README.TXT
cp ../rockbot ./DC/rockbot
cp ../editor ./DC/rockbot
rsync -r --exclude=.svn ../data ./DC/rockbot
cd ./DC
tar -cvf ../Rockbot_DC_$VERSIONNAME.tar rockbot
gzip ../Rockbot_DC_$VERSIONNAME.tar
