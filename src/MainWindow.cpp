#include "MainWindow.h"
#include "MapEditorWidget.h"

#include <QtGui/QComboBox>
#include <QtGui/QListWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDockWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtGui/QStatusBar>


namespace WZMapEditor
{

MainWindow::MainWindow()
{
	this->setGeometry(0, 0, 800, 600);
	this->setWindowState(Qt::WindowMaximized);
	this->setUnifiedTitleAndToolBarOnMac(true);

// ADD ACTIONS
	actFileNew = new QAction(QIcon::fromTheme("document-new"), tr("&New"), this);
	actFileNew->setShortcut(QKeySequence::New);
	actFileOpen = new QAction(QIcon::fromTheme("document-open"), tr("&Open..."), this);
	actFileOpen->setShortcut(QKeySequence::Open);
	actFileSave = new QAction(QIcon::fromTheme("document-save"), tr("&Save..."), this);
	actFileSave->setShortcut(QKeySequence::Save);
	actFileSaveAs = new QAction(QIcon::fromTheme("document-save-as"), tr("&Save As..."), this);
	actFileSaveAs->setShortcut(QKeySequence::SaveAs);
	actFileExit = new QAction(                                   tr("&Exit"), this);
	actFileExit->setShortcut(QKeySequence::Quit);

	actViewFullScreen = new QAction(QIcon::fromTheme("view-fullscreen"), tr("Full &Screen"), this);
	actViewFullScreen->setCheckable(true);
	actViewFullScreen->setShortcut(Qt::Key_F11);
	actViewDockTileset = new QAction(tr("Tileset dock"), this);
	actViewDockTileset->setCheckable(true);
	actViewDockTileset->setShortcut(Qt::CTRL + Qt::Key_1);
	actViewDockTerrain = new QAction(tr("Terrain dock"), this);
	actViewDockTerrain->setCheckable(true);
	actViewDockTerrain->setShortcut(Qt::CTRL + Qt::Key_2);
	actViewDockLand = new QAction(tr("Land dock"), this);
	actViewDockLand->setCheckable(true);
	actViewDockLand->setShortcut(Qt::CTRL + Qt::Key_3);
	actViewDockTriangle = new QAction(tr("Triangle dock"), this);
	actViewDockTriangle->setCheckable(true);
	actViewDockTriangle->setShortcut(Qt::CTRL + Qt::Key_4);
	actViewDockMap = new QAction(tr("Map dock"), this);
	actViewDockMap->setCheckable(true);
	actViewDockMap->setShortcut(Qt::CTRL + Qt::Key_5);
	actViewDockObjects = new QAction(tr("Objects dock"), this);
	actViewDockObjects->setCheckable(true);
	actViewDockObjects->setShortcut(Qt::CTRL + Qt::Key_6);

	actRaiseDockTileset  = new QAction(this);
	actRaiseDockTileset->setShortcut(Qt::Key_1);
	actRaiseDockTerrain  = new QAction(this);
	actRaiseDockTerrain->setShortcut(Qt::Key_2);
	actRaiseDockLand     = new QAction(this);
	actRaiseDockLand->setShortcut(Qt::Key_3);
	actRaiseDockTriangle = new QAction(this);
	actRaiseDockTriangle->setShortcut(Qt::Key_4);
	actRaiseDockMap      = new QAction(this);
	actRaiseDockMap->setShortcut(Qt::Key_5);
	actRaiseDockObjects  = new QAction(this);
	actRaiseDockObjects->setShortcut(Qt::Key_6);

	this->addAction(actRaiseDockTileset);
	this->addAction(actRaiseDockTerrain);
	this->addAction(actRaiseDockLand);
	this->addAction(actRaiseDockTriangle);
	this->addAction(actRaiseDockMap);
	this->addAction(actRaiseDockObjects);

	actToolsPreferences = new QAction(tr("&Preferences"), this);
	actToolsPreferences->setShortcut(QKeySequence::Preferences);

	actHelpAboutQt = new QAction(tr("About &Qt"), this);

// ADD NEW MENUS

	menuFile  = this->menuBar()->addMenu(tr("&File"));
	menuFile->addAction(actFileNew);
	menuFile->addAction(actFileOpen);
	menuFile->addAction(actFileSave);
	menuFile->addAction(actFileSaveAs);
	menuFile->addSeparator();
	menuFile->addAction(actFileExit);


	menuEdit = this->menuBar()->addMenu(tr("&Edit"));

	menuView  = this->menuBar()->addMenu(tr("&View"));
	menuView->addAction(actViewDockTileset);
	menuView->addAction(actViewDockTerrain);
	menuView->addAction(actViewDockLand);
	menuView->addAction(actViewDockTriangle);
	menuView->addAction(actViewDockMap);
	menuView->addAction(actViewDockObjects);
	menuView->addSeparator();
	menuView->addAction(actViewFullScreen);

	menuTools = this->menuBar()->addMenu(tr("&Tools"));
	menuTools->addAction(actToolsPreferences);

	menuHelp  = this->menuBar()->addMenu(tr("&Help"));
	menuHelp->addAction(actHelpAboutQt);

// CREATE TOOLBARS
	toolMain = new QToolBar("Main Toolbar", this);
	toolMain->addAction(actFileNew);
	toolMain->addAction(actFileOpen);
	toolMain->addAction(actFileSave);
	toolMain->addSeparator();
	toolMain->addAction(actViewFullScreen);
	this->addToolBar(toolMain);

// CREATE DOCKS

	dockTileset  = new QDockWidget(tr("Tileset"),  this, 0);
	dockTerrain  = new QDockWidget(tr("Terrain"),  this, 0);
	dockLand     = new QDockWidget(tr("Land"),     this, 0);
	dockTriangle = new QDockWidget(tr("Triangle"), this, 0);
	dockMap      = new QDockWidget(tr("Map"),      this, 0);
	dockObjects  = new QDockWidget(tr("Objects"),  this, 0);

	// CREATE DOCK "TILESET" WIDGETS
	QGridLayout *layTileset  = new QGridLayout;
	QLabel *labelTileset     = new QLabel(tr("Tileset: "), this);
	QLabel *labelTilesetType = new QLabel(tr("Tileset Type: "), this);

	comboTileset     = new QComboBox(this);
	comboTilesetType = new QComboBox(this);
	listTileset = new QListWidget(this);
	listTileset->setViewMode(QListView::IconMode);
	listTileset->setResizeMode(QListView::Adjust);
	listTileset->setDragEnabled(false);
	listTileset->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	listTileset->setSpacing(0);

	layTileset->setAlignment(Qt::AlignTop);
	layTileset->setContentsMargins(0, 0, 0, 0);
	layTileset->setSpacing(0);
	layTileset->setColumnStretch(1, 100);
	layTileset->addWidget(labelTileset,     1, 0);
	layTileset->addWidget(comboTileset,     1, 1);
	layTileset->addWidget(labelTilesetType, 2, 0);
	layTileset->addWidget(comboTilesetType, 2, 1);
	layTileset->addWidget(listTileset,      3, 0, 1, 2);

	QWidget *dockTilesetContents = new QWidget(this);
	dockTilesetContents->setLayout(layTileset);
	dockTileset->setWidget(dockTilesetContents);

	// CREATE DOCK "TERRAIN" WIDGETS
	QGridLayout *layTerrain = new QGridLayout;
	QLabel *labelTerrainGroundType = new QLabel(tr("Ground Type: "), this);
	QLabel *labelTerrainRoadType   = new QLabel(tr("Road Type: "), this);

	comboTerrainGroundType = new QComboBox(this);
	comboTerrainRoadType   = new QComboBox(this);

	layTerrain->setAlignment(Qt::AlignTop);
	layTerrain->setContentsMargins(0, 0, 0, 0);
	layTerrain->setSpacing(0);
	layTerrain->setColumnStretch(1, 100);
	layTerrain->addWidget(labelTerrainGroundType, 1, 0);
	layTerrain->addWidget(comboTerrainGroundType, 1, 1);
	layTerrain->addWidget(labelTerrainRoadType,   2, 0);
	layTerrain->addWidget(comboTerrainRoadType,   2, 1);

	QWidget *dockTerrainContents = new QWidget(this);
	dockTerrainContents->setLayout(layTerrain);
	dockTerrain->setWidget(dockTerrainContents);

	// ADD DOCKS TO MAINWINDOW
	this->addDockWidget(Qt::LeftDockWidgetArea, dockTileset);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockTerrain);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockLand);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockTriangle);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockMap);
	this->addDockWidget(Qt::LeftDockWidgetArea, dockObjects);

	this->setTabPosition(Qt::LeftDockWidgetArea,  QTabWidget::West);
	this->setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);
	this->tabifyDockWidget(dockObjects,  dockMap);
	this->tabifyDockWidget(dockMap,      dockTriangle);
	this->tabifyDockWidget(dockTriangle, dockLand);
	this->tabifyDockWidget(dockLand,     dockTerrain);
	this->tabifyDockWidget(dockTerrain,  dockTileset);



////////// TEMPORARY USE ONLY |||||||||| JUST FOR PREVIEW \\\\\\\\\\

	// ICONS FOR TEXTURES LIST
	QStringList freedesktop;
	freedesktop << "address-book-new" << "application-exit" << "document-new" << "document-open" << "document-save" << "document-open-recent"
			<< "document-page-setup"<<"document-print"<<"document-properties"<<"document-revert"<<"document-save-as"<<"edit-copy"<<"edit-cut"
			<< "edit-delete"<<"edit-find";

#define MAX_LIST freedesktop.size()
	QListWidgetItem *bla[MAX_LIST];
	for (int i = 0; i < MAX_LIST; i++)
	{
		bla[i] = new QListWidgetItem(QIcon::fromTheme(freedesktop[i]), 0, listTileset, 0);
		listTileset->addItem(bla[i]);
	}

	// TILESET NAMES

	comboTileset->addItem(tr("Arizona"));
	comboTileset->addItem(tr("New York"));
	comboTileset->addItem(tr("Chicago"));
	comboTileset->addItem(tr("Moscow"));
	comboTilesetType->addItem(tr("Sand"));
	comboTilesetType->addItem(tr("Grass"));
	comboTilesetType->addItem(tr("Ice"));
	comboTilesetType->addItem(tr("Nuclear Winter"));
comboTerrainGroundType->addItem(tr("Red"));
comboTerrainGroundType->addItem(tr("Yellow"));
comboTerrainGroundType->addItem(tr("Sand"));
comboTerrainGroundType->addItem(tr("Brown"));
comboTerrainRoadType->addItem(tr("Road"));
comboTerrainRoadType->addItem(tr("Track"));
////////// TEMPORARY USE ONLY |||| END |||| JUST FOR PREVIEW \\\\\\\\\\

	statusBar = new QStatusBar(this);
	statusCoordsX = new QLabel(this);
	statusCoordsY = new QLabel(this);
	statusBar->addWidget(statusCoordsX);
	statusBar->addWidget(statusCoordsY);
	statusBar->setSizeGripEnabled(true);

	this->setStatusBar(statusBar);

// SET CENTRAL WIDGET

	mapEdit = new MapEditorWidget(this);
	this->setCentralWidget(mapEdit);

// CONNECT SIGNALS AND SLOTS

	connect(actFileNew,        SIGNAL(triggered()),   this, SLOT(onActionFileNew()));
	connect(actFileOpen,       SIGNAL(triggered()),   this, SLOT(onActionFileOpen()));
	connect(actFileSave,       SIGNAL(triggered()),   this, SLOT(onActionFileSave()));
	connect(actFileSaveAs,     SIGNAL(triggered()),   this, SLOT(onActionFileSaveAs()));
	connect(actFileExit,       SIGNAL(triggered()),   this, SLOT(close()));
	connect(actViewDockTileset,  SIGNAL(toggled(bool)), dockTileset,  SLOT(setShown(bool)));
	connect(actViewDockTerrain,  SIGNAL(toggled(bool)), dockTerrain,  SLOT(setShown(bool)));
	connect(actViewDockLand,     SIGNAL(toggled(bool)), dockLand,     SLOT(setShown(bool)));
	connect(actViewDockTriangle, SIGNAL(toggled(bool)), dockTriangle, SLOT(setShown(bool)));
	connect(actViewDockMap,      SIGNAL(toggled(bool)), dockMap,      SLOT(setShown(bool)));
	connect(actViewDockObjects,  SIGNAL(toggled(bool)), dockObjects,  SLOT(setShown(bool)));
	connect(actViewFullScreen,   SIGNAL(toggled(bool)), this,         SLOT(onActionChangeFullScreen(bool)));
	connect(actRaiseDockTileset,  SIGNAL(triggered()), dockTileset, SLOT(raise()));
	connect(actRaiseDockTerrain,  SIGNAL(triggered()), dockTerrain, SLOT(raise()));
	connect(actRaiseDockLand,     SIGNAL(triggered()), dockLand, SLOT(raise()));
	connect(actRaiseDockTriangle, SIGNAL(triggered()), dockTriangle, SLOT(raise()));
	connect(actRaiseDockMap,      SIGNAL(triggered()), dockMap, SLOT(raise()));
	connect(actRaiseDockObjects,  SIGNAL(triggered()), dockObjects, SLOT(raise()));
	connect(actHelpAboutQt,    SIGNAL(triggered()),   this, SLOT(onActionHelpAboutQt()));

	connect(mapEdit, SIGNAL(changeCoordsX(int)), this, SLOT(setStatusCoordsX(int)));
	connect(mapEdit, SIGNAL(changeCoordsY(int)), this, SLOT(setStatusCoordsY(int)));
	//this->onMenuViewFullScreen_click(false);

	actViewDockTileset->setChecked(true);
	actViewDockTerrain->setChecked(true);
	actViewDockLand->setChecked(true);
	actViewDockTriangle->setChecked(true);
	actViewDockMap->setChecked(true);
	actViewDockObjects->setChecked(true);
}

void MainWindow::onActionChangeFullScreen (bool value)
{
	switch (value)
	{
	case true:
		this->setWindowState(this->windowState() | Qt::WindowFullScreen);
		break;

	case false:
		this->setWindowState(this->windowState() ^ Qt::WindowFullScreen);
		break;
	}
}

void MainWindow::onActionHelpAboutQt ()
{
	QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::onActionFileNew ()
{

}

void MainWindow::onActionFileOpen ()
{

}

void MainWindow::onActionFileSave ()
{

}

void MainWindow::onActionFileSaveAs ()
{

}

void MainWindow::setStatusCoordsX (int coords)
{
	statusCoordsX->setText(QString("x:%1").arg(coords));
}

void MainWindow::setStatusCoordsY (int coords)
{
	statusCoordsY->setText(QString("y:%1").arg(coords));
}

}
