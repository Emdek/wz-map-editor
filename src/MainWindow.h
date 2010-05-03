#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>


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

	QMenu   *menuFile;
	QAction *menuFileOpen;
	QAction *menuFileExit;
	QMenu   *menuEdit;
	QMenu   *menuView;
	QAction *menuViewFullScreen;
	QMenu   *menuTools;
	QAction *menuToolsPreferences;
	QMenu   *menuHelp;
	QAction *menuHelpAboutQt;

	QDockWidget *dockTerrain;
	QComboBox   *comboTileset;
	QComboBox   *comboTilesetType;
	QListWidget *listTileset;

public slots:
	void onMenuViewFullScreen_click (bool value);

};

}

#endif // MAINWINDOW_H
