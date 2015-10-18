#!/bin/sh

rm -r -f ./win32/rockbot/data
rsync -r --exclude=.svn ../../../data ./win32/rockbot/
cd win32
zip -r ../Rockbot_WIN32_0.1a1.zip *
