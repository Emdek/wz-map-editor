# -------------------------------------------------
# Project created by QtCreator 2010-05-03T15:23:43
# -------------------------------------------------
QT += opengl
TARGET = WZMapEditor
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += src/main.cpp \
    src/MainWindow.cpp \
    src/MapEditorWidget.cpp \
    src/ActionManager.cpp \
    src/ToolBarManager.cpp \
    src/ShortcutManager.cpp \
    src/ShortcutEditorDelegate.cpp \
    src/ShortcutEditorWidget.cpp \
    src/ToolBarWidget.cpp \
    src/SettingManager.cpp \
    src/MapParser.cpp \
    src/MapInformation.cpp \
    src/Tileset.cpp \
    src/PreferencesManager.cpp \
    src/FileSystemCompleterModel.cpp
HEADERS += src/MainWindow.h \
    src/MapEditorWidget.h \
    src/ActionManager.h \
    src/ToolBarManager.h \
    src/ShortcutManager.h \
    src/ShortcutEditorDelegate.h \
    src/ShortcutEditorWidget.h \
    src/ToolBarWidget.h \
    src/SettingManager.h \
    src/MapParser.h \
    src/MapInformation.h \
    src/Tileset.h \
    src/PreferencesManager.h \
    src/FileSystemCompleterModel.h
FORMS += src/ui/MainWindow.ui \
    src/ui/ToolBarEditorDialog.ui \
    src/ui/ShortcutEditorDialog.ui \
    src/ui/ObjectsDockWidget.ui \
    src/ui/MapSettingsDialog.ui \
    src/ui/LandDockWidget.ui \
    src/ui/TilesetDockWidget.ui \
    src/ui/TerrainDockWidget.ui \
    src/ui/PreferencesDialog.ui
RESOURCES += src/resources/resources.qrc
