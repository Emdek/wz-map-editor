# -------------------------------------------------
# Project created by QtCreator 2010-05-03T15:23:43
# -------------------------------------------------
QT += xml
TARGET = WZMapEditor
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += src/main.cpp \
	src/MainWindow.cpp \
	src/Map2DViewWidget.cpp \
	src/Map3DViewWidget.cpp \
	src/ActionManager.cpp \
	src/ToolBarManager.cpp \
	src/ShortcutManager.cpp \
	src/ShortcutEditorDelegate.cpp \
	src/ShortcutEditorWidget.cpp \
	src/ToolBarWidget.cpp \
	src/SettingManager.cpp \
	src/MapParser.cpp \
	src/Map.cpp \
	src/Tileset.cpp \
	src/PreferencesManager.cpp \
	src/FileSystemCompleterModel.cpp \
	src/MapWriter.cpp \
	src/MapSettingsManager.cpp
HEADERS += src/MainWindow.h \
	src/Map2DViewWidget.h \
	src/Map3DViewWidget.h \
	src/ActionManager.h \
	src/ToolBarManager.h \
	src/ShortcutManager.h \
	src/ShortcutEditorDelegate.h \
	src/ShortcutEditorWidget.h \
	src/ToolBarWidget.h \
	src/SettingManager.h \
	src/MapParser.h \
	src/Map.h \
	src/Tileset.h \
	src/PreferencesManager.h \
	src/FileSystemCompleterModel.h \
	src/MapWriter.h \
	src/MapSettingsManager.h
FORMS += src/ui/MainWindow.ui \
	src/ui/ToolBarEditorDialog.ui \
	src/ui/ShortcutEditorDialog.ui \
	src/ui/ObjectsDockWidget.ui \
	src/ui/MapSettingsDialog.ui \
	src/ui/InformationDockWidget.ui \
	src/ui/TilesetDockWidget.ui \
	src/ui/TerrainDockWidget.ui \
	src/ui/PreferencesDialog.ui \
	src/ui/Map2DEditorWidget.ui \
	src/ui/Map3DEditorWidget.ui
RESOURCES += src/resources/resources.qrc
DISTFILES += WZMapEditor.xcodeproj \
	Resources \
	configs \
	README.BUILD.MAC.txt
