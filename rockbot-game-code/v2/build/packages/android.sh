#!/bin/sh
VERSIONNAME=`cat version_name.txt`

read -r -p "Did you remember to update data version in AndroidAppSettings.cfg? [y/N] " response
case $response in
	[yY][eE][sS]|[yY]) 
		rm -r -f ./Android
		mkdir ./Android
		mkdir ./Android/data
		rsync -r --exclude=.svn ../data ./Android
		rm ./Android/data/game212.sav
		rm ./Android/data/config_v212.sav
		cd ./Android
		zip -r ./data_$VERSIONNAME.zip ./data
		rm ../../../../Android/project/jni/application/src/AndroidData/*.zip
		cp ./data_$VERSIONNAME.zip ../../../../Android/project/jni/application/src/AndroidData/
		rm ../../../../Android/project/jni/application/src/libapplication.so
		rm ../../../../Android/project/obj/local/armeabi/libapplication.so
		rm ../../../../Android/project/libs/armeabi/libapplication.so
		cp ../../libapplication.so ../../../Android/project/jni/application/src/libapplication.so
		cd ../../../../Android
		LINENUMBER=`grep -n "AppDataDownloadUrl" AndroidAppSettings.cfg | cut -f1 -d:`
		sed $LINENUMBER'c\'"AppDataDownloadUrl=\"Game data is about 4.8 Mb|data_$VERSIONNAME.zip\"" AndroidAppSettings.cfg > AndroidAppSettings.cfg.new
		cp AndroidAppSettings.cfg AndroidAppSettings.cfg.old
		cp AndroidAppSettings.cfg.new AndroidAppSettings.cfg
		sh ./build.sh -s release
		cp ./project/bin/MainActivity-debug.apk ../v1/build/packages/Rockbot_Android_$VERSIONNAME.apk
		;;
	*)
		echo "Aborted."
		;;
esac
