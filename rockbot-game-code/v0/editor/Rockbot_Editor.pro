# -------------------------------------------------
# Project created by QtCreator 2009-12-03T12:16:29
# -------------------------------------------------
TARGET = ../bin/editor
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    editorarea.cpp \
    editortilepallete.cpp \
    mediator.cpp \
    addwizard.cpp \
    npcpreviewarea.cpp \
    loadgamepicker.cpp \
    dialognpcedit.cpp \
    dialogobjectedit.cpp \
    projectilepreviewarea.cpp
HEADERS += mainwindow.h \
    editorarea.h \
    editortilepallete.h \
    mediator.h \
    format/file_map.h \
    format/file_game.h \
    defines.h \
    addwizard.h \
    ../common/format/file_game.h \
    ../common/format/file_map.h \
    addwizard.h \
    ../common/format/file_npc.h \
    npcpreviewarea.h \
    loadgamepicker.h \
    ../game/include/types.h \
    dialognpcedit.h \
    ../game/include/sound_ps2.h \
    ../game/include/sound_nds.h \
    ../game/include/config.h \
    ../common/format/file_npc.h \
    ../common/format/file_map.h \
    ../common/format/file_game.h \
    dialogobjectedit.h \
    ../common/format/file_structs.h \
    projectilePreviewArea.h \
    projectilepreviewarea.h
FORMS += mainwindow.ui \
    addwizard.ui \
    loadgamepicker.ui \
    dialognpcedit.ui \
    dialogobjectedit.ui
RESOURCES += resources/icons/icons.qrc
CONFIG += console
INCLUDEPATH += ../common
OTHER_FILES += ../docs/RoadMap \
    ../docs/ChangeLog.txt
