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
		cp ./project/bin/MainActivity-release-unsigned.apk ../v1.0/build/packages/TEMP_Rockbot_Android_$VERSIONNAME.apk
		#remove as assinaturas do Android (porque ele está colocando uma do além)
		zip -d ../v1.0/build/packages/TEMP_Rockbot_Android_$VERSIONNAME.apk META-INF/*
		#assina e realinha o zpk
		jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ~/.android/my-release-key.keystore  ../v1.0/build/packages/TEMP_Rockbot_Android_$VERSIONNAME.apk alias_name
		zipalign -v 4 ../v1.0/build/packages/TEMP_Rockbot_Android_$VERSIONNAME.apk ../v1.0/build/packages/Rockbot_Android_$VERSIONNAME.apk
		;;
	*)
		echo "Aborted."
		;;
esac

#jarsigner -verbose -sigalg SHA1withRSA -digestalg SHA1 -keystore ~/.android/my-release-key.keystore ./Rockbot_Android_1.0.2.apk alias_name
#zipalign [-f] [-v] <alignment> infile.apk outfile.apk