#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QLabel>

namespace Ui
{
	class MainWindow;
	class TilesetDockWidget;
	class TerrainDockWidget;
	class LandDockWidget;
	class TriangleDockWidget;
	class ObjectsDockWidget;
}

namespace WZMapEditor
{

class ToolBarWidget;

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
	void updateCoordinates(int x, int y, int z);

protected:
	void closeEvent(QCloseEvent *event);

private:
	Ui::MainWindow *m_mainWindowUi;
	Ui::TilesetDockWidget *m_tilesetUi;
	Ui::TerrainDockWidget *m_terrainUi;
	Ui::LandDockWidget *m_landUi;
	Ui::TriangleDockWidget *m_triangleUi;
	Ui::ObjectsDockWidget *m_objectsUi;

	QLabel *m_coordinatesLabel;
};

}

#endif // MAINWINDOW_H
