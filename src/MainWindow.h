#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Tileset.h"

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>


namespace Ui
{
	class MainWindow;
	class Map2DEditorWidget;
	class TilesetDockWidget;
	class ObjectsDockWidget;
	class TerrainDockWidget;
	class InformationDockWidget;
}

namespace WZMapEditor
{

class ToolBarWidget;
class Tileset;
class Map;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = NULL);
	~MainWindow();

	bool openFile(const QString &fileName);
	bool canClose();

public slots:
	void actionNew();
	void actionOpen();
	void actionOpenRecent(QAction *action);
	void actionClearRecentFiles();
	void actionProperties(bool newMap = false);
	void actionFullscreen(bool checked);
	void action3DView(bool checked);
	void actionZoomIn();
	void actionZoomOut();
	void actionZoomOriginal();
	void actionShortcutsConfiguration();
	void actionToolbarsConfiguration();
	void actionApplicationConfiguration();
	void actionAboutApplication();
	void actionLockToolBars(bool lock);
	void actionToggleDock();
	void updateRecentFilesMenu();
	void updateTilesetView();
	void updateCoordinates(int x, int y, int z);
	void updateZoom(int zoom);

protected:
	void closeEvent(QCloseEvent *event);

private:
	Map *m_map;
	QList<QDockWidget*> m_docks;
	QSlider *m_zoomSlider;
	QLabel *m_coordinatesLabel;
	QLabel *m_fileNameLabel;
	TilesetType m_currentTileset;
	Ui::MainWindow *m_mainWindowUi;
	Ui::Map2DEditorWidget *m_map2DEditorWidgetUi;
	Ui::TilesetDockWidget *m_tilesetUi;
	Ui::ObjectsDockWidget *m_objectsUi;
	Ui::TerrainDockWidget *m_terrainUi;
	Ui::InformationDockWidget *m_informationUi;
};

}

#endif // MAINWINDOW_H
