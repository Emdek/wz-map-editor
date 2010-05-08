#include "MainWindow.h"
#include "MapEditorWidget.h"
#include "ui_MainWindow.h"
#include "ui_TilesetDockWidget.h"
#include "ui_TerrainDockWidget.h"
#include "ui_LandDockWidget.h"
#include "ui_TriangleDockWidget.h"
#include "ui_ObjectsDockWidget.h"

#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtGui/QDockWidget>
#include <QtGui/QMessageBox>


namespace WZMapEditor
{

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_mainWindowUi(new Ui::MainWindow()),
	m_tilesetUi(new Ui::TilesetDockWidget()),
	m_terrainUi(new Ui::TerrainDockWidget()),
	m_landUi(new Ui::LandDockWidget()),
	m_triangleUi(new Ui::TriangleDockWidget()),
	m_objectsUi(new Ui::ObjectsDockWidget()),
	m_coordinatesLabel(new QLabel(this))
{
	m_mainWindowUi->setupUi(this);

	setWindowState(Qt::WindowMaximized);

	QDockWidget *tilesetDockWidget = new QDockWidget(tr("Tileset"), this);
	QDockWidget *terrainDockWidget = new QDockWidget(tr("Terrain"), this);
	QDockWidget *landDockWidget = new QDockWidget(tr("Land"), this);
	QDockWidget *triangleDockWidget = new QDockWidget(tr("Triangle"), this);
	QDockWidget *objectsDockWidget = new QDockWidget(tr("Objects"), this);

	tilesetDockWidget->setObjectName("tilesetDockWidget");
	terrainDockWidget->setObjectName("terrainDockWidget");
	landDockWidget->setObjectName("landDockWidget");
	triangleDockWidget->setObjectName("triangleDockWidget");
	objectsDockWidget->setObjectName("objectsDockWidget");

	tilesetDockWidget->setWidget(new QWidget(tilesetDockWidget));
	terrainDockWidget->setWidget(new QWidget(terrainDockWidget));
	landDockWidget->setWidget(new QWidget(landDockWidget));
	triangleDockWidget->setWidget(new QWidget(triangleDockWidget));
	objectsDockWidget->setWidget(new QWidget(objectsDockWidget));

	m_tilesetUi->setupUi(tilesetDockWidget->widget());
	m_terrainUi->setupUi(terrainDockWidget->widget());
	m_landUi->setupUi(landDockWidget->widget());
	m_triangleUi->setupUi(triangleDockWidget->widget());
	m_objectsUi->setupUi(objectsDockWidget->widget());

	addDockWidget(Qt::LeftDockWidgetArea, tilesetDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, terrainDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, landDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, triangleDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, objectsDockWidget);

	setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);
	setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);

	tabifyDockWidget(tilesetDockWidget, terrainDockWidget);
	tabifyDockWidget(terrainDockWidget, landDockWidget);
	tabifyDockWidget(landDockWidget, triangleDockWidget);
	tabifyDockWidget(triangleDockWidget, objectsDockWidget);

	tilesetDockWidget->raise();

	m_mainWindowUi->mainToolBar->addAction(m_mainWindowUi->actionNew);
	m_mainWindowUi->mainToolBar->addAction(m_mainWindowUi->actionOpen);
	m_mainWindowUi->mainToolBar->addAction(m_mainWindowUi->actionSave);
	m_mainWindowUi->mainToolBar->addSeparator();
	m_mainWindowUi->mainToolBar->addAction(m_mainWindowUi->actionFullscreen);

	m_mainWindowUi->statusBar->addPermanentWidget(m_coordinatesLabel);

	connect(m_mainWindowUi->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_mainWindowUi->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(actionFullscreen(bool)));
	connect(m_mainWindowUi->actionTileset, SIGNAL(toggled(bool)), tilesetDockWidget, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionTerrain, SIGNAL(toggled(bool)), terrainDockWidget, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionLand, SIGNAL(toggled(bool)), landDockWidget, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionTriangle, SIGNAL(toggled(bool)), triangleDockWidget, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionObjects, SIGNAL(toggled(bool)), objectsDockWidget, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionAboutApplication, SIGNAL(triggered()), this, SLOT(actionAboutApplication()));
	connect(m_mainWindowUi->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));
	connect(m_mainWindowUi->map3DViewWidget, SIGNAL(cooridantesChanged(int, int, int)), this, SLOT(updateCoordinates(int, int, int)));
	connect(tilesetDockWidget, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionTileset, SLOT(setChecked(bool)));
	connect(terrainDockWidget, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionTerrain, SLOT(setChecked(bool)));
	connect(landDockWidget, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionLand, SLOT(setChecked(bool)));
	connect(triangleDockWidget, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionTriangle, SLOT(setChecked(bool)));
	connect(objectsDockWidget, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionObjects, SLOT(setChecked(bool)));

	restoreGeometry(QSettings().value("geometry").toByteArray());
	restoreState(QSettings().value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
	delete m_mainWindowUi;
	delete m_tilesetUi;
	delete m_terrainUi;
	delete m_landUi;
	delete m_triangleUi;
	delete m_objectsUi;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings().setValue("geometry", saveGeometry());
	QSettings().setValue("windowState", saveState());

	QMainWindow::closeEvent(event);
}

void MainWindow::actionFullscreen(bool checked)
{
	if (checked)
	{
		setWindowState(this->windowState() | Qt::WindowFullScreen);
	}
	else
	{
		setWindowState(this->windowState() ^ Qt::WindowFullScreen);
	}
}

void MainWindow::actionAboutApplication()
{
	QMessageBox::about(this, tr("About Warzone 2100 Map Editor"), QString(tr("<b>Warzone 2100 Map Editor %1</b><br />Map viewer and editor for Warzone 2100.").arg(QApplication::instance()->applicationVersion())));
}

void MainWindow::updateCoordinates(int x, int y, int z)
{
	m_coordinatesLabel->setText(QString("x: %1 y: %2 z: %3").arg(x).arg(y).arg(z));
}

}
