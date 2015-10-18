#-------------------------------------------------
#
# Project created by QtCreator 2011-03-05T22:53:17
#
#-------------------------------------------------

QT       -= core
QT       -= gui

CONFIG += win32
#CONFIG += linux
#CONFIG += dingux
#CONFIG += win32_cross # cross compiling win32 on linux using mingw32 (NOT FUNCTIONAL YET) #

CONFIG += console
CONFIG -= app_bundle

TARGET = rockbot

linux {
	LIBS = -L/usr/X11R6/lib \
		-lX11 \
		-lSDL_mixer \
		-lSDL_image \
                -lSDL_ttf \
                `sdl-config \
                --libs`

	INCLUDES = -I/usr/include/SDL \
		-I/usr/include \
		-I. \
		-I./include \
		-L/usr/lib
	QMAKE_CCFLAGS += -DLINUX -Wno-reorder
	QMAKE_CXXFLAGS += -DLINUX -Wno-reorder
}

win32 {
        LIBS =  -lSDL_mixer \
                -lSDL_image \
                -lSDL_ttf \
                                -lmingw32 -lSDLmain -lSDL -mwindows


		INCLUDES = -I/usr/include/SDL \
                -I/usr/include \
                -I. \
                -I./include \
                -L/usr/lib
        QMAKE_CCFLAGS += -DWIN32
        QMAKE_CXXFLAGS += -DWIN32
}

win32_cross {
		TARGET = rockbot.exe
		LIBS = `/opt/SDL-1.2.13/bin/sdl-config --libs` -lSDL_image -lSDL_ttf -lSDL_mixer

		INCLUDES = -I/opt/SDL-1.2.13/include \
				-I. \
				-I./include \
				-L/usr/lib

		QMAKE_CC = /opt/cross-tools/bin/i386-mingw32msvc-gcc
		QMAKE_CXX = /opt/cross-tools/bin/i386-mingw32msvc-g++
		QMAKE_LINK = /opt/cross-tools/bin/i386-mingw32msvc-ld
		QMAKE_CCFLAGS += -DWIN32 -I/opt/SDL-1.2.13/include `/opt/SDL-1.2.13/bin/sdl-config --cflags`
		QMAKE_CXXFLAGS += -DWIN32 -I/opt/SDL-1.2.13/include `/opt/SDL-1.2.13/bin/sdl-config --cflags`
}

dingux {
	TARGET = rockbot.dge
	QMAKE_CC = mipsel-linux-gcc
	QMAKE_CXX = mipsel-linux-g++
	QMAKE_LINK = mipsel-linux-g++

	QMAKE_CCFLAGS += -pipe -g -Wall -W -D_REENTRANT -DDINGUX -DHANDHELD
	QMAKE_CXXFLAGS += -pipe -g -Wall -W -D_REENTRANT -DDINGUX -DHANDHELD

	LIBS = $(SUBLIBS)  -L$(DINGUX_TOOLCHAIN_PREFIX)/lib -lSDL_mixer -lSDL_image -lSDL_ttf `sdl-config --libs` -lpthread

	INCLUDES = -I. -I../include -I.
}

open_pandora {
	TARGET = rockbot

	QMAKE_CCFLAGS += -pipe -g -Wall -W -D_REENTRANT -DOPEN_PANDORA -DHANDHELD
	QMAKE_CXXFLAGS += -pipe -g -Wall -W -D_REENTRANT -DOPEN_PANDORA -DHANDHELD

	LIBS = $(SUBLIBS)  -L$(DINGUX_TOOLCHAIN_PREFIX)/lib -lSDL_mixer -lSDL_image -lSDL_ttf `sdl-config --libs` -lpthread

	INCLUDES = -I. -I../include -I.
}


TEMPLATE = app


SOURCES += main.cpp \
    character.cpp \
    graphicslib.cpp \
    inputlib.cpp \
    game.cpp \
    stage.cpp \
    classmap.cpp \
    classplayer.cpp \
    timerlib.cpp \
    sceneslib.cpp \
    soundlib.cpp \
    projectilelib.cpp \
    classnpc.cpp \
    stage_select.cpp \
    class_config.cpp \
    objects/object.cpp \
    class_colorcycle.cpp \
    graphic/option_picker.cpp \
    scenes/dialogs.cpp \
    file/file_io.cpp \
    character/classboss.cpp \
    graphic/animation.cpp \
    scenes/ending.cpp \
    colision_detection.cpp

HEADERS += \
    character.h \
    st_hitPoints.h \
    st_platform.h \
    st_teleporter.h \
    st_common.h \
    st_characterState.h \
    st_projectile.h \
    graphicslib.h \
    defines.h \
    inputlib.h \
    game.h \
    stage.h \
    classmap.h \
    classplayer.h \
    timerlib.h \
    sceneslib.h \
    soundlib.h \
    projectilelib.h \
    classnpc.h \
    stage_select.h \
    class_config.h \
    objects/object.h \
    class_colorcycle.h \
    graphic/option_picker.h \
    scenes/dialogs.h \
    file/format.h \
    file/file_io.h \
    character/classboss.h \
    graphic/animation.h \
    scenes/ending.h \
    file/v_2_0_0.h \
    file/v_1.h \
    file/v_2_0_1.h \
    file/v_2_0_2.h \
    colision_detection.h

OTHER_FILES += \
    docs/RoadMap.txt \
    rockbot_dingux.pro









