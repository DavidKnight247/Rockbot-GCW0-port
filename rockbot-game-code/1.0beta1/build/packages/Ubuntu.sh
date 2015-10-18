#/usr/bin
mkdir ubuntu
mkdir ubuntu/game
cd ubuntu/game
rm -r -f ./debian/opt/rockbot
mkdir -p ./debian/opt/rockbot
rm -r -f ./debian/opt/rockbot/data
rsync -r --exclude=.svn ../../../data ./debian/opt/rockbot
cp ../../../rockbot ./debian/opt/rockbot/
cp ../../../editor ./debian/opt/rockbot/
cp ../../../conversor ./debian/opt/rockbot/
mkdir -p ./debian/DEBIAN
cp ../../control ./debian/DEBIAN

dpkg-deb --build debian/ ../../Rockbot_1.0a1.deb
