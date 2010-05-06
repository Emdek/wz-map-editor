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
    src/ToolBarWidget.cpp
HEADERS += src/MainWindow.h \
    src/MapEditorWidget.h \
    src/ActionManager.h \
    src/ToolBarManager.h \
    src/ShortcutManager.h \
    src/ShortcutEditorDelegate.h \
    src/ShortcutEditorWidget.h \
    src/ToolBarWidget.h
FORMS += src/ui/MainWindow.ui \
    src/ui/ToolBarEditorDialog.ui \
    src/ui/ShortcutEditorDialog.ui
RESOURCES += src/resources/resources.qrc
