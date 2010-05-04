#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QLabel>


class QComboBox;
class QListWidget;

namespace WZMapEditor
{

class MapEditorWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow ();

	MapEditorWidget *mapEdit;

	QToolBar *toolMain;

	QMenu   *menuFile;
	QMenu   *menuEdit;
	QMenu   *menuView;
	QMenu   *menuTools;
	QMenu   *menuHelp;
	QAction *actFileNew;
	QAction *actFileOpen;
	QAction *actFileSave;
	QAction *actFileSaveAs;
	QAction *actFileExit;
	QAction *actViewDockTileset;
	QAction *actViewDockTerrain;
	QAction *actViewDockLand;
	QAction *actViewDockTriangle;
	QAction *actViewDockMap;
	QAction *actViewDockObjects;
	QAction *actViewFullScreen;
	QAction *actToolsPreferences;
	QAction *actHelpAboutQt;

	QAction *actRaiseDockTileset;
	QAction *actRaiseDockTerrain;
	QAction *actRaiseDockLand;
	QAction *actRaiseDockTriangle;
	QAction *actRaiseDockMap;
	QAction *actRaiseDockObjects;

	QDockWidget *dockTileset;
	QComboBox   *comboTileset;
	QComboBox   *comboTilesetType;
	QDockWidget *dockTerrain;
	QComboBox   *comboTerrainGroundType;
	QComboBox   *comboTerrainRoadType;
	QDockWidget *dockLand;
	QDockWidget *dockTriangle;
	QDockWidget *dockMap;
	QDockWidget *dockObjects;
	QListWidget *listTileset;

	QStatusBar  *statusBar;
	QLabel      *statusCoordsX;
	QLabel      *statusCoordsY;

public slots:
	void onActionFileNew          ();
	void onActionFileOpen         ();
	void onActionFileSave         ();
	void onActionFileSaveAs       ();
	void onActionChangeFullScreen (bool value);
	void onActionHelpAboutQt      ();

	void setStatusCoordsX (int coords);
	void setStatusCoordsY (int coords);
};

}

#endif // MAINWINDOW_H
