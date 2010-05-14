#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QDockWidget>
#include <QtGui/QLabel>


namespace Ui
{
	class MainWindow;
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

public slots:
	void actionFullscreen(bool checked);
	void actionShortcutsConfiguration();
	void actionToolbarsConfiguration();
	void actionApplicationConfiguration();
	void actionAboutApplication();
	void actionLockToolBars(bool lock);
	void actionToggleDock();
	void showTileset(int tileset = -1);
	void updateCoordinates(int x, int y, int z);

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::MainWindow *m_mainWindowUi;
	Ui::TilesetDockWidget *m_tilesetUi;
	Ui::TerrainDockWidget *m_terrainUi;
	Ui::LandDockWidget *m_landUi;
	Ui::ObjectsDockWidget *m_objectsUi;
	MapInformation *m_mapInformation;
	QList<QDockWidget*> m_docks;
	QList<Tileset*> m_tilesets;
	QLabel *m_coordinatesLabel;
};

}

#endif // MAINWINDOW_H
