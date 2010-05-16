#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>
#include <QtGui/QLabel>
#include <QtGui/QSlider>


namespace Ui
{
	class MainWindow;
	class Map2DEditorWidget;
	class TilesetDockWidget;
	class TerrainDockWidget;
	class LandDockWidget;
	class ObjectsDockWidget;
}

namespace WZMapEditor
{

class ToolBarWidget;
class Tileset;
class MapInformation;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = NULL);
	~MainWindow();

	bool canClose();

public slots:
	void actionNew();
	void actionProperties();
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
	void updateTilesetView();
	void updateCoordinates(int x, int y, int z);
	void updateZoom(int zoom);

protected:
	void closeEvent(QCloseEvent *event);

private:
	MapInformation *m_mapInformation;
	QList<QDockWidget*> m_docks;
	QSlider *m_zoomSlider;
	QLabel *m_coordinatesLabel;
	Ui::MainWindow *m_mainWindowUi;
	Ui::Map2DEditorWidget *m_map2DEditorWidgetUi;
	Ui::TilesetDockWidget *m_tilesetUi;
	Ui::TerrainDockWidget *m_terrainUi;
	Ui::LandDockWidget *m_landUi;
	Ui::ObjectsDockWidget *m_objectsUi;
};

}

#endif // MAINWINDOW_H
