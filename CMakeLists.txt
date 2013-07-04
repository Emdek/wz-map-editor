project(wzme)

cmake_minimum_required(VERSION 2.8.9)

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_ui(wzme_UI
	src/ui/MainWindow.ui
	src/ui/ToolBarEditorDialog.ui
	src/ui/ShortcutEditorDialog.ui
	src/ui/ObjectsDockWidget.ui
	src/ui/MapSettingsDialog.ui
	src/ui/InformationDockWidget.ui
	src/ui/TilesetDockWidget.ui
	src/ui/TerrainDockWidget.ui
	src/ui/PreferencesDialog.ui
	src/ui/Map2DEditorWidget.ui
	src/ui/Map3DEditorWidget.ui
)

qt5_add_resources(wzme_QRC
	src/resources/resources.qrc
)

add_executable(wzme
	${wzme_UI}
	${wzme_QRC}
	src/main.cpp
	src/MainWindow.cpp
	src/MainWindow.cpp
	src/Map2DViewWidget.cpp
	src/Map3DViewWidget.cpp
	src/ActionsManager.cpp
	src/ToolBarsManager.cpp
	src/ShortcutsManager.cpp
	src/ShortcutEditorDelegate.cpp
	src/ShortcutEditorWidget.cpp
	src/ToolBarWidget.cpp
	src/SettingsManager.cpp
	src/MapParser.cpp
	src/Map.cpp
	src/Tileset.cpp
	src/PreferencesManager.cpp
	src/FileSystemCompleterModel.cpp
	src/MapWriter.cpp
	src/MapSettingsManager.cpp
)

qt5_use_modules(wzme Gui Widgets Xml)
