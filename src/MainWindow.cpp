#include "MainWindow.h"
#include "MapEditorWidget.h"

#include <QtGui/QComboBox>
#include <QtGui/QListWidget>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDockWidget>


namespace WZMapEditor
{

MainWindow::MainWindow()
{
	this->setGeometry(0, 0, 800, 600);
	this->setWindowState(Qt::WindowMaximized);
	this->setUnifiedTitleAndToolBarOnMac(true);

// ADD NEW MENUS

	menuFile  = this->menuBar()->addMenu(tr("&File"));
	menuFileOpen = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
	menuFileExit = new QAction(                                   tr("&Exit"), this);

	menuFileOpen->setShortcut(QKeySequence::Open);
	menuFileExit->setShortcut(QKeySequence::Quit);

	menuFile->addAction(menuFileOpen);
	menuFile->addSeparator();
	menuFile->addAction(menuFileExit);

	menuEdit = this->menuBar()->addMenu(tr("&Edit"));

	menuView  = this->menuBar()->addMenu(tr("&View"));
	menuViewFullScreen = new QAction(tr("Full &Screen"), this);
	menuViewFullScreen->setCheckable(true);

	menuViewFullScreen->setShortcut(Qt::Key_F11);

	menuView->addAction(menuViewFullScreen);

	menuTools = this->menuBar()->addMenu(tr("&Tools"));
	menuToolsPreferences = new QAction(tr("&Preferences"), this);

	menuToolsPreferences->setShortcut(QKeySequence::Preferences);

	menuTools->addAction(menuToolsPreferences);

	menuHelp  = this->menuBar()->addMenu(tr("&Help"));
	menuHelpAboutQt = new QAction(tr("About &Qt"), this);
	menuHelp->addAction(menuHelpAboutQt);

// CREATE DOCKS

	dockTerrain = new QDockWidget(tr("Terrain"), this, 0);

	QVBoxLayout *layTerrain = new QVBoxLayout;
	// CREATE DOCK TERRAIN WIDGETS
	comboTileset     = new QComboBox(this);
	comboTilesetType = new QComboBox(this);
	layTerrain->setAlignment(Qt::AlignTop);
	layTerrain->setContentsMargins(0, 0, 0, 0);
	layTerrain->setSpacing(0);
	layTerrain->addWidget(comboTileset);
	layTerrain->addWidget(comboTilesetType);

	QWidget *dockTerrainContents = new QWidget(this);
	dockTerrainContents->setLayout(layTerrain);
	dockTerrain->setWidget(dockTerrainContents);

	this->addDockWidget(Qt::LeftDockWidgetArea, dockTerrain);

	listTileset = new QListWidget(this);
	listTileset->setViewMode(QListView::IconMode);
	listTileset->setResizeMode(QListView::Adjust);
	listTileset->setDragEnabled(false);
	listTileset->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	listTileset->setSpacing(0);
	layTerrain->addWidget(listTileset);



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

////////// TEMPORARY USE ONLY |||| END |||| JUST FOR PREVIEW \\\\\\\\\\

// SET CENTRAL WIDGET

	mapEdit = new MapEditorWidget(this);
	this->setCentralWidget(mapEdit);

// CONNECT SIGNALS AND SLOTS

	connect(menuFileExit,       SIGNAL(triggered()),   this, SLOT(close()));
	connect(menuViewFullScreen, SIGNAL(toggled(bool)), this, SLOT(onMenuViewFullScreen_click(bool)));
	//this->onMenuViewFullScreen_click(false);
}

void MainWindow::onMenuViewFullScreen_click (bool value)
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

}
