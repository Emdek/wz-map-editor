#include "MainWindow.h"
#include "SettingManager.h"
#include "ActionManager.h"
#include "ShortcutManager.h"
#include "ToolBarManager.h"
#include "PreferencesManager.h"

#include "ui_MainWindow.h"
#include "ui_TilesetDockWidget.h"
#include "ui_TerrainDockWidget.h"
#include "ui_LandDockWidget.h"
#include "ui_ObjectsDockWidget.h"

#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>


namespace WZMapEditor
{

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_mainWindowUi(new Ui::MainWindow()),
	m_tilesetUi(new Ui::TilesetDockWidget()),
	m_terrainUi(new Ui::TerrainDockWidget()),
	m_landUi(new Ui::LandDockWidget()),
	m_objectsUi(new Ui::ObjectsDockWidget()),
	m_coordinatesLabel(new QLabel(this))
{
	SettingManager::createInstance(this);
	ActionManager::createInstance(this);

	m_mainWindowUi->setupUi(this);

	setWindowState(Qt::WindowMaximized);

	QDockWidget *tilesetDockWidget = new QDockWidget(tr("Tileset"), this);
	QDockWidget *terrainDockWidget = new QDockWidget(tr("Terrain"), this);
	QDockWidget *landDockWidget = new QDockWidget(tr("Land"), this);
	QDockWidget *objectsDockWidget = new QDockWidget(tr("Objects"), this);

	tilesetDockWidget->setObjectName("tilesetDockWidget");
	terrainDockWidget->setObjectName("terrainDockWidget");
	landDockWidget->setObjectName("landDockWidget");
	objectsDockWidget->setObjectName("objectsDockWidget");

	tilesetDockWidget->setWidget(new QWidget(tilesetDockWidget));
	terrainDockWidget->setWidget(new QWidget(terrainDockWidget));
	landDockWidget->setWidget(new QWidget(landDockWidget));
	objectsDockWidget->setWidget(new QWidget(objectsDockWidget));

	m_tilesetUi->setupUi(tilesetDockWidget->widget());
	m_terrainUi->setupUi(terrainDockWidget->widget());
	m_landUi->setupUi(landDockWidget->widget());
	m_objectsUi->setupUi(objectsDockWidget->widget());

	addDockWidget(Qt::LeftDockWidgetArea, tilesetDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, terrainDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, landDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, objectsDockWidget);

	m_docks.append(tilesetDockWidget);
	m_docks.append(terrainDockWidget);
	m_docks.append(landDockWidget);
	m_docks.append(objectsDockWidget);

	setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);
	setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);

	tabifyDockWidget(tilesetDockWidget, terrainDockWidget);
	tabifyDockWidget(terrainDockWidget, landDockWidget);
	tabifyDockWidget(landDockWidget, objectsDockWidget);

	tilesetDockWidget->raise();

	m_mainWindowUi->statusBar->addPermanentWidget(m_coordinatesLabel);

	m_mainWindowUi->actionNew->setIcon(QIcon::fromTheme("document-new", QIcon(":/icons/document-new.png")));
	m_mainWindowUi->actionOpen->setIcon(QIcon::fromTheme("document-open", QIcon(":/icons/document-open.png")));
	m_mainWindowUi->menuOpenRecent->setIcon(QIcon::fromTheme("document-open-recent", QIcon(":/icons/document-open-recent.png")));
	m_mainWindowUi->actionClearRecentFiles->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/icons/edit-clear.png")));
	m_mainWindowUi->actionSave->setIcon(QIcon::fromTheme("document-save", QIcon(":/icons/document-save.png")));
	m_mainWindowUi->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as", QIcon(":/icons/document-save-as.png")));
	m_mainWindowUi->actionExit->setIcon(QIcon::fromTheme("application-exit", QIcon(":/icons/application-exit.png")));
	m_mainWindowUi->actionUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/icons/edit-undo.png")));
	m_mainWindowUi->actionRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/icons/edit-redo.png")));
	m_mainWindowUi->actionFullscreen->setIcon(QIcon::fromTheme("view-fullscreen", QIcon(":/icons/view-fullscreen.png")));

	m_mainWindowUi->actionShortcutsConfiguration->setIcon(QIcon::fromTheme("configure-shortcuts", QIcon(":/icons/configure-shortcuts.png")));
	m_mainWindowUi->actionToolbarsConfiguration->setIcon(QIcon::fromTheme("configure-toolbars", QIcon(":/icons/configure-toolbars.png")));
	m_mainWindowUi->actionApplicationConfiguration->setIcon(QIcon::fromTheme("configure", QIcon(":/icons/configure.png")));
	m_mainWindowUi->actionHelp->setIcon(QIcon::fromTheme("help-contents", QIcon(":/icons/help-contents.png")));
	m_mainWindowUi->actionAboutApplication->setIcon(QIcon(":/icons/warzone2100mapeditor.png"));
	m_mainWindowUi->actionAboutQt->setIcon(QIcon(":/icons/qt.png"));

	ActionManager::registerAction(m_mainWindowUi->actionNew);
	ActionManager::registerAction(m_mainWindowUi->actionOpen);
	ActionManager::registerAction(m_mainWindowUi->actionOpenRecent);
	ActionManager::registerAction(m_mainWindowUi->actionSave);
	ActionManager::registerAction(m_mainWindowUi->actionSaveAs);
	ActionManager::registerAction(m_mainWindowUi->actionExit);
	ActionManager::registerAction(m_mainWindowUi->actionUndo);
	ActionManager::registerAction(m_mainWindowUi->actionRedo);
	ActionManager::registerAction(m_mainWindowUi->actionFullscreen);
	ActionManager::registerAction(m_mainWindowUi->actionTileset);
	ActionManager::registerAction(m_mainWindowUi->actionTerrain);
	ActionManager::registerAction(m_mainWindowUi->actionLand);
	ActionManager::registerAction(m_mainWindowUi->actionObjects);
	ActionManager::registerAction(m_mainWindowUi->actionShortcutsConfiguration);
	ActionManager::registerAction(m_mainWindowUi->actionToolbarsConfiguration);
	ActionManager::registerAction(m_mainWindowUi->actionApplicationConfiguration);
	ActionManager::registerAction(m_mainWindowUi->actionHelp);
	ActionManager::registerAction(m_mainWindowUi->actionAboutQt);
	ActionManager::registerAction(m_mainWindowUi->actionAboutApplication);

	connect(m_mainWindowUi->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_mainWindowUi->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(actionFullscreen(bool)));
	connect(m_mainWindowUi->actionTileset, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionTerrain, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionLand, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionObjects, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionMainToolbar, SIGNAL(toggled(bool)), m_mainWindowUi->mainToolbar, SLOT(setVisible(bool)));
	connect(m_mainWindowUi->actionShortcutsConfiguration, SIGNAL(triggered()), this, SLOT(actionShortcutsConfiguration()));
	connect(m_mainWindowUi->actionToolbarsConfiguration, SIGNAL(triggered()), this, SLOT(actionToolbarsConfiguration()));
	connect(m_mainWindowUi->actionApplicationConfiguration, SIGNAL(triggered()), this, SLOT(actionApplicationConfiguration()));
	connect(m_mainWindowUi->actionAboutApplication, SIGNAL(triggered()), this, SLOT(actionAboutApplication()));
	connect(m_mainWindowUi->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));
	connect(m_mainWindowUi->map3DViewWidget, SIGNAL(cooridantesChanged(int, int, int)), this, SLOT(updateCoordinates(int, int, int)));
	connect(m_mainWindowUi->mainToolbar, SIGNAL(visibilityChanged(bool)), m_mainWindowUi->actionMainToolbar, SLOT(setChecked(bool)));

	m_mainWindowUi->mainToolbar->reload();

	actionLockToolBars(QSettings().value("actionLockToolBars", false).toBool());

	restoreGeometry(QSettings().value("geometry").toByteArray());
	restoreState(QSettings().value("windowState").toByteArray());
}

MainWindow::~MainWindow()
{
	delete m_mainWindowUi;
	delete m_tilesetUi;
	delete m_terrainUi;
	delete m_landUi;
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

void MainWindow::actionShortcutsConfiguration()
{
	new ShortcutManager(this);
}

void MainWindow::actionToolbarsConfiguration()
{
	ToolBarWidget *toolbar = NULL;
	QList<ToolBarWidget*> toolbars;
	toolbars.append(m_mainWindowUi->mainToolbar);

	if (sender()->inherits("ToolBarWidget"))
	{
		toolbar = qobject_cast<ToolBarWidget*>(sender());
	}

	new ToolBarManager(toolbars, toolbar, this);
}

void MainWindow::actionApplicationConfiguration()
{
	new PreferencesManager(this);
}

void MainWindow::actionAboutApplication()
{
	QMessageBox::about(this, tr("About Warzone 2100 Map Editor"), QString(tr("<b>Warzone 2100 Map Editor %1</b><br />Map viewer and editor for Warzone 2100.").arg(QApplication::instance()->applicationVersion())));
}

void MainWindow::actionLockToolBars(bool lock)
{
	QSettings().setValue("toolBarsLocked", lock);

	m_mainWindowUi->mainToolbar->setMovable(!lock);
}

void MainWindow::actionToggleDock()
{
	QAction *action = qobject_cast<QAction*>(sender());

	if (!action)
	{
		return;
	}

	QString name = action->objectName().mid(6);

	for (int i = 0; i < m_docks.count(); ++i)
	{
		if (m_docks.at(i)->objectName().startsWith(name, Qt::CaseInsensitive))
		{
			if (m_docks.at(i)->isVisible())
			{
				if (childAt(m_docks.at(i)->pos()) == m_docks.at(i))
				{
					m_docks.at(i)->close();
				}
				else
				{
					m_docks.at(i)->raise();
				}
			}
			else
			{
				m_docks.at(i)->show();
				m_docks.at(i)->raise();
			}

			break;
		}

	}
}

void MainWindow::updateCoordinates(int x, int y, int z)
{
	m_coordinatesLabel->setText(QString("x: %1 y: %2 z: %3").arg(x).arg(y).arg(z));
}

}
