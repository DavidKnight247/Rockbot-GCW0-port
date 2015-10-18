# -------------------------------------------------
# Project created by QtCreator 2009-11-28T17:11:52
# -------------------------------------------------
QT -= core \
    gui
TARGET = ../bin/rockbot
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
LIBS = -L/usr/X11R6/lib \
    -lX11 \
    -lSDL_mixer \
    -lSDL_image \
    -lSDL_ttf \
    `sdl-config \
    --libs`
INCLUDEPATH += ../common
INCLUDES = -I/usr/include/SDL \
    -I/usr/include \
    -I. \
    -I./include \
    -L/usr/lib
SOURCES = game.c
HEADERS += game.h \
    input.h \
    graphics.h \
    common.h \
    intro.h \
    ../common/shared.h \
    ps2_modules.h \
    ../common/player_and_npc.h \
    projectiles.h \
    include/sound.h \
    include/types.h \
    include/sound_ps2.h \
    include/sound_nds.h \
    include/config.h \
    include/headers.h \
    include/transfer.h \
    object.h \
    npcs.h \
    ../common/dialogs.h \
    ia_bosses.h \
OTHER_FILES += ../docs/ReadMe \
    ../docs/ReadMe \
    ../docs/RoadMap \
    ../docs/ChangeLog.txt \
    ../common/format/file.h \
    ../common/format/logic.h \
    ../common/format/format.h \
    include/preload.h \
    ia.h \
    ../common/shared_IA.h \
    ../common/shared_trajectories.h
QMAKE_CFLAGS_DEBUG += -D LINUX \
-D PC
QMAKE_CFLAGS_RELEASE += -D LINUX \
-D PC
