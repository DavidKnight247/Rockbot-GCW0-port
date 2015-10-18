# -------------------------------------------------
# Project created by QtCreator 2009-12-03T12:16:29
# -------------------------------------------------
QT       += core gui
TARGET = editor
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
    ../file/file_io.cpp \
    mainwindow_tab/object_tab.cpp \
    mainwindow_tab/npc_edit.cpp \
    mainwindow_tab/ai_tab.cpp \
    mainwindow_tab/weapon_edit.cpp \
    dialog_pick_color.cpp \
    projectilepreviewarea.cpp \
    common.cpp \
    player_preview_area.cpp
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
    ../common/shared.h \
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
    ../common/format/file_object.h \
    dialogobjectedit.h \
    ../file/file_io.h \
    ../file/format.h \
    mainwindow_tab/object_tab.h \
    mainwindow_tab/npc_edit.h \
    mainwindow_tab/ai_tab.h \
    mainwindow_tab/weapon_edit.h \
    dialog_pick_color.h \
    projectilepreviewarea.h \
    common.h \
    player_preview_area.h
FORMS += mainwindow.ui \
    addwizard.ui \
    loadgamepicker.ui \
    dialognpcedit.ui \
    dialogobjectedit.ui \
    mainwindow_tab/object_tab.ui \
    mainwindow_tab/npc_edit.ui \
    mainwindow_tab/ai_tab.ui \
    dialog_pick_color.ui
LIBS = -lmingw32 -mwindows -lqtmaind
CONFIG += CONSOLE
RESOURCES += resources/icons/icons.qrc
CONFIG += console
INCLUDEPATH += ../common
OTHER_FILES += ../docs/RoadMap \
    ../docs/ChangeLog.txt
