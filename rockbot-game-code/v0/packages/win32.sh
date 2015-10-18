#!/bin/sh

sh ../bin/pack_data.sh
rm -r -f ./win32/rockbot/data
cp -r ./data ./win32/rockbot
cd win32
zip -r ../Rockbot_WIN32.zip *
