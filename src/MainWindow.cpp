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

	actViewFullScreen = new QAction(tr("Full &Screen"), this);
	actViewFullScreen->setCheckable(true);
	actViewFullScreen->setShortcut(Qt::Key_F11);

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
	this->addToolBar(toolMain);

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
	connect(actViewFullScreen, SIGNAL(toggled(bool)), this, SLOT(onActionChangeFullScreen(bool)));
	connect(actHelpAboutQt,    SIGNAL(triggered()),   this, SLOT(onActionHelpAboutQt()));

	connect(mapEdit, SIGNAL(changeCoordsX(int)), this, SLOT(setStatusCoordsX(int)));
	connect(mapEdit, SIGNAL(changeCoordsY(int)), this, SLOT(setStatusCoordsY(int)));
	//this->onMenuViewFullScreen_click(false);
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
