#include "MainWindow.h"
#include "ActionsManager.h"
#include "SettingsManager.h"
#include "ShortcutsManager.h"
#include "ToolBarsManager.h"
#include "ToolBarWidget.h"
#include "PreferencesManager.h"
#include "MapSettingsManager.h"
#include "Map.h"
#include "MapParser.h"

#include "ui_MainWindow.h"
#include "ui_Map2DEditorWidget.h"
#include "ui_TilesetDockWidget.h"
#include "ui_ObjectsDockWidget.h"
#include "ui_TerrainDockWidget.h"
#include "ui_InformationDockWidget.h"

#include <QtCore/QStandardPaths>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QToolButton>

#define ApplicationName "Warzone 2100 Map Editor"

namespace WZMapEditor
{

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	m_map(new Map(this)),
	m_zoomSlider(new QSlider(Qt::Horizontal, this)),
	m_coordinatesLabel(new QLabel(this)),
	m_fileNameLabel(new QLabel(tr("Unnamed"), this)),
	m_currentTileset(NoTileset),
	m_mainWindowUi(new Ui::MainWindow()),
	m_map2DEditorWidgetUi(new Ui::Map2DEditorWidget()),
	m_tilesetUi(new Ui::TilesetDockWidget()),
	m_objectsUi(new Ui::ObjectsDockWidget()),
	m_terrainUi(new Ui::TerrainDockWidget()),
	m_informationUi(new Ui::InformationDockWidget())
{
	QStringList toolBars;
	toolBars << "mainToolBar" << "visibilityToolBar";

	QStringList mainToolBar;
	mainToolBar	<< "New" << "Open" << "Save" << QString() << "Fullscreen" << "3DView";

	QStringList visibilityToolBar;
	visibilityToolBar << "ToggleStructuresVisibility" << "ToggleDroidsVisibility" << "ToggleFeaturesVisibility" << QString() << "ToggleTexturesVisibility" << "ToggleHeightIndicatorsVisibility" << QString() << "ToggleScrollLimitsVisibility" << "ToggleGatewaysVisibility";

	SettingsManager::createInstance(this);
	SettingsManager::setDefaultValue("dataPath", QVariant(QString()));
	SettingsManager::setDefaultValue("tileSize", QVariant(64));
	SettingsManager::setDefaultValue("zoomLevel", QVariant(100));
	SettingsManager::setDefaultValue("3DView", QVariant(true));
	SettingsManager::setDefaultValue("lockToolBars", QVariant(false));
	SettingsManager::setDefaultValue("toolBars", QVariant(toolBars));
	SettingsManager::setDefaultValue("ToolBars/mainToolBar/actions", QVariant(mainToolBar));
	SettingsManager::setDefaultValue("ToolBars/mainToolBar/title", QVariant(tr("Main Toolbar")));
	SettingsManager::setDefaultValue("ToolBars/visibilityToolBar/actions", QVariant(visibilityToolBar));
	SettingsManager::setDefaultValue("ToolBars/visibilityToolBar/title", QVariant(tr("Visibility Toolbar")));
	SettingsManager::setDefaultValue("Actions/New", QVariant(QKeySequence(QKeySequence::New).toString()));
	SettingsManager::setDefaultValue("Actions/Open", QVariant(QKeySequence(QKeySequence::Open).toString()));
	SettingsManager::setDefaultValue("Actions/Save", QVariant(QKeySequence(QKeySequence::Save).toString()));
	SettingsManager::setDefaultValue("Actions/SaveAs", QVariant(QKeySequence(QKeySequence::SaveAs).toString()));
	SettingsManager::setDefaultValue("Actions/Exit", QVariant(QKeySequence(QKeySequence::Quit).toString()));
	SettingsManager::setDefaultValue("Actions/Undo", QVariant(QKeySequence(QKeySequence::Undo).toString()));
	SettingsManager::setDefaultValue("Actions/Redo", QVariant(QKeySequence(QKeySequence::Redo).toString()));
	SettingsManager::setDefaultValue("Actions/ZoomIn", QVariant(QKeySequence(QKeySequence::ZoomIn).toString()));
	SettingsManager::setDefaultValue("Actions/ZoomOut", QVariant(QKeySequence(QKeySequence::ZoomOut).toString()));
	SettingsManager::setDefaultValue("Actions/Help", QVariant(QKeySequence(QKeySequence::HelpContents).toString()));
	SettingsManager::setDefaultValue("Actions/Tileset", QVariant(QKeySequence("Ctrl+1").toString()));
	SettingsManager::setDefaultValue("Actions/Objects", QVariant(QKeySequence("Ctrl+2").toString()));
	SettingsManager::setDefaultValue("Actions/Terrain", QVariant(QKeySequence("Ctrl+3").toString()));
	SettingsManager::setDefaultValue("Actions/Information", QVariant(QKeySequence("Ctrl+4").toString()));
	SettingsManager::setDefaultValue("Actions/ApplicationConfiguration", QVariant(QKeySequence(QKeySequence::Preferences).toString()));
	SettingsManager::setDefaultValue("Actions/Fullscreen", QVariant(QKeySequence("F11").toString()));

	m_mainWindowUi->setupUi(this);

	setWindowTitle(tr("%1 - Unnamed").arg(ApplicationName));

	QDockWidget *tilesetDockWidget = new QDockWidget(tr("Tileset"), this);
	QDockWidget *objectsDockWidget = new QDockWidget(tr("Objects"), this);
	QDockWidget *terrainDockWidget = new QDockWidget(tr("Terrain"), this);
	QDockWidget *informationDockWidget = new QDockWidget(tr("Information"), this);

	tilesetDockWidget->setObjectName("tilesetDockWidget");
	objectsDockWidget->setObjectName("objectsDockWidget");
	terrainDockWidget->setObjectName("terrainDockWidget");
	informationDockWidget->setObjectName("informationDockWidget");

	tilesetDockWidget->setWidget(new QWidget(tilesetDockWidget));
	objectsDockWidget->setWidget(new QWidget(objectsDockWidget));
	terrainDockWidget->setWidget(new QWidget(terrainDockWidget));
	informationDockWidget->setWidget(new QWidget(informationDockWidget));

	m_map2DEditorWidgetUi->setupUi(m_mainWindowUi->map2DEditorWidget);
	m_tilesetUi->setupUi(tilesetDockWidget->widget());
	m_objectsUi->setupUi(objectsDockWidget->widget());
	m_terrainUi->setupUi(terrainDockWidget->widget());
	m_informationUi->setupUi(informationDockWidget->widget());

	addDockWidget(Qt::LeftDockWidgetArea, tilesetDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, objectsDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, terrainDockWidget);
	addDockWidget(Qt::LeftDockWidgetArea, informationDockWidget);

	m_docks.append(tilesetDockWidget);
	m_docks.append(objectsDockWidget);
	m_docks.append(terrainDockWidget);
	m_docks.append(informationDockWidget);

	setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::West);
	setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);

	tabifyDockWidget(tilesetDockWidget, objectsDockWidget);
	tabifyDockWidget(objectsDockWidget, informationDockWidget);
	tabifyDockWidget(informationDockWidget, terrainDockWidget);

	tilesetDockWidget->raise();

	Tileset::load(this);

	m_coordinatesLabel->setMinimumWidth(150);

	m_zoomSlider->setRange(10, 500);
	m_zoomSlider->setMaximumWidth(200);

	QToolButton *zoomInButton = new QToolButton(this);
	zoomInButton->setDefaultAction(m_mainWindowUi->actionZoomIn);
	zoomInButton->setAutoRepeat(true);

	QToolButton *zoomOutButton = new QToolButton(this);
	zoomOutButton->setDefaultAction(m_mainWindowUi->actionZoomOut);
	zoomOutButton->setAutoRepeat(true);

	m_mainWindowUi->statusBar->addPermanentWidget(m_fileNameLabel);
	m_mainWindowUi->statusBar->addPermanentWidget(m_coordinatesLabel);
	m_mainWindowUi->statusBar->addPermanentWidget(zoomOutButton);
	m_mainWindowUi->statusBar->addPermanentWidget(m_zoomSlider);
	m_mainWindowUi->statusBar->addPermanentWidget(zoomInButton);

	QActionGroup *mouseMode = new QActionGroup(this);
	mouseMode->addAction(m_mainWindowUi->actionMouseModeView);
	mouseMode->addAction(m_mainWindowUi->actionMouseModeSelect);
	mouseMode->addAction(m_mainWindowUi->actionMouseModeMove);
	mouseMode->addAction(m_mainWindowUi->actionMouseModePaint);
	mouseMode->setExclusive(true);

	m_mainWindowUi->actionNew->setIcon(QIcon::fromTheme("document-new", QIcon(":/icons/document-new.png")));
	m_mainWindowUi->actionOpen->setIcon(QIcon::fromTheme("document-open", QIcon(":/icons/document-open.png")));
	m_mainWindowUi->menuOpenRecent->setIcon(QIcon::fromTheme("document-open-recent", QIcon(":/icons/document-open-recent.png")));
	m_mainWindowUi->actionClearRecentFiles->setIcon(QIcon::fromTheme("edit-clear", QIcon(":/icons/edit-clear.png")));
	m_mainWindowUi->actionSave->setIcon(QIcon::fromTheme("document-save", QIcon(":/icons/document-save.png")));
	m_mainWindowUi->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as", QIcon(":/icons/document-save-as.png")));
	m_mainWindowUi->actionProperties->setIcon(QIcon::fromTheme("document-properties", QIcon(":/icons/document-properties.png")));
	m_mainWindowUi->actionExit->setIcon(QIcon::fromTheme("application-exit", QIcon(":/icons/application-exit.png")));
	m_mainWindowUi->actionUndo->setIcon(QIcon::fromTheme("edit-undo", QIcon(":/icons/edit-undo.png")));
	m_mainWindowUi->actionRedo->setIcon(QIcon::fromTheme("edit-redo", QIcon(":/icons/edit-redo.png")));
	m_mainWindowUi->actionFullscreen->setIcon(QIcon::fromTheme("view-fullscreen", QIcon(":/icons/view-fullscreen.png")));
	m_mainWindowUi->action3DView->setIcon(QIcon(":/icons/view-3d.png"));
	m_mainWindowUi->actionZoomIn->setIcon(QIcon::fromTheme("zoom-in", QIcon(":/icons/zoom-in.png")));
	m_mainWindowUi->actionZoomOut->setIcon(QIcon::fromTheme("zoom-out", QIcon(":/icons/zoom-out.png")));
	m_mainWindowUi->actionZoomOriginal->setIcon(QIcon::fromTheme("zoom-original", QIcon(":/icons/zoom-original.png")));
	m_mainWindowUi->actionMouseModeView->setIcon(QIcon(":/icons/input-mouse.png"));
	m_mainWindowUi->actionMouseModeSelect->setIcon(QIcon(":/icons/edit-select.png"));
	m_mainWindowUi->actionMouseModeMove->setIcon(QIcon(":/icons/transform-move.png"));;
	m_mainWindowUi->actionMouseModePaint->setIcon(QIcon(":/icons/tool-pencil.png"));;
	m_mainWindowUi->actionShortcutsConfiguration->setIcon(QIcon::fromTheme("configure-shortcuts", QIcon(":/icons/configure-shortcuts.png")));
	m_mainWindowUi->actionToolbarsConfiguration->setIcon(QIcon::fromTheme("configure-toolbars", QIcon(":/icons/configure-toolbars.png")));
	m_mainWindowUi->actionApplicationConfiguration->setIcon(QIcon::fromTheme("configure", QIcon(":/icons/configure.png")));
	m_mainWindowUi->actionHelp->setIcon(QIcon::fromTheme("help-contents", QIcon(":/icons/help-contents.png")));
	m_mainWindowUi->actionAboutApplication->setIcon(QIcon(":/icons/warzone2100mapeditor.png"));
	m_mainWindowUi->actionAboutQt->setIcon(QIcon(":/icons/qt.png"));

	m_tilesetUi->tileTypeComboBox->setItemData(0, AllTiles);
	m_tilesetUi->tileTypeComboBox->setItemData(1, GroundTile);
	m_tilesetUi->tileTypeComboBox->setItemData(2, WaterTile);
	m_tilesetUi->tileTypeComboBox->setItemData(3, RoadTile);
	m_tilesetUi->tileTypeComboBox->setItemData(4, TracksTile);
	m_tilesetUi->tileTypeComboBox->setItemData(5, CraterTile);
	m_tilesetUi->tileTypeComboBox->setItemData(6, CliffTile);
	m_tilesetUi->tileTypeComboBox->setItemData(7, TransitionTile);
	m_tilesetUi->tileTypeComboBox->setItemData(8, OtherTile);

	ActionsManager::createInstance(this);
	ActionsManager::registerAction(m_mainWindowUi->actionNew);
	ActionsManager::registerAction(m_mainWindowUi->actionOpen);
	ActionsManager::registerAction(m_mainWindowUi->actionOpenRecent);
	ActionsManager::registerAction(m_mainWindowUi->actionSave);
	ActionsManager::registerAction(m_mainWindowUi->actionSaveAs);
	ActionsManager::registerAction(m_mainWindowUi->actionProperties);
	ActionsManager::registerAction(m_mainWindowUi->actionExit);
	ActionsManager::registerAction(m_mainWindowUi->actionUndo);
	ActionsManager::registerAction(m_mainWindowUi->actionRedo);
	ActionsManager::registerAction(m_mainWindowUi->actionFullscreen);
	ActionsManager::registerAction(m_mainWindowUi->action3DView);
	ActionsManager::registerAction(m_mainWindowUi->actionZoomIn);
	ActionsManager::registerAction(m_mainWindowUi->actionZoomOut);
	ActionsManager::registerAction(m_mainWindowUi->actionZoomOriginal);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleStructuresVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleDroidsVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleFeaturesVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleTexturesVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleHeightIndicatorsVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleScrollLimitsVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionToggleGatewaysVisibility);
	ActionsManager::registerAction(m_mainWindowUi->actionTileset);
	ActionsManager::registerAction(m_mainWindowUi->actionTerrain);
	ActionsManager::registerAction(m_mainWindowUi->actionInformation);
	ActionsManager::registerAction(m_mainWindowUi->actionObjects);
	ActionsManager::registerAction(m_mainWindowUi->actionMouseModeView);
	ActionsManager::registerAction(m_mainWindowUi->actionMouseModeSelect);
	ActionsManager::registerAction(m_mainWindowUi->actionMouseModeMove);
	ActionsManager::registerAction(m_mainWindowUi->actionMouseModePaint);
	ActionsManager::registerAction(m_mainWindowUi->actionShortcutsConfiguration);
	ActionsManager::registerAction(m_mainWindowUi->actionToolbarsConfiguration);
	ActionsManager::registerAction(m_mainWindowUi->actionApplicationConfiguration);
	ActionsManager::registerAction(m_mainWindowUi->actionHelp);
	ActionsManager::registerAction(m_mainWindowUi->actionAboutQt);
	ActionsManager::registerAction(m_mainWindowUi->actionAboutApplication);

	Tileset::createCache(ArizonaTileset, SettingsManager::getValue("tileSize").toInt());

	updateTilesetView();

	connect(m_mainWindowUi->actionNew, SIGNAL(triggered()), this, SLOT(actionNew()));
	connect(m_mainWindowUi->actionOpen, SIGNAL(triggered()), this, SLOT(actionOpen()));
	connect(m_mainWindowUi->menuOpenRecent, SIGNAL(triggered(QAction*)), this, SLOT(actionOpenRecent(QAction*)));
	connect(m_mainWindowUi->actionClearRecentFiles, SIGNAL(triggered()), this, SLOT(actionClearRecentFiles()));
	connect(m_mainWindowUi->actionProperties, SIGNAL(triggered()), this, SLOT(actionProperties()));
	connect(m_mainWindowUi->actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(m_mainWindowUi->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(actionFullscreen(bool)));
	connect(m_mainWindowUi->action3DView, SIGNAL(triggered(bool)), this, SLOT(action3DView(bool)));
	connect(m_mainWindowUi->actionZoomIn, SIGNAL(triggered()), this, SLOT(actionZoomIn()));
	connect(m_mainWindowUi->actionZoomOut, SIGNAL(triggered()), this, SLOT(actionZoomOut()));
	connect(m_mainWindowUi->actionZoomOriginal, SIGNAL(triggered()), this, SLOT(actionZoomOriginal()));
	connect(m_mainWindowUi->actionTileset, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionTerrain, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionInformation, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionObjects, SIGNAL(triggered()), this, SLOT(actionToggleDock()));
	connect(m_mainWindowUi->actionShortcutsConfiguration, SIGNAL(triggered()), this, SLOT(actionShortcutsConfiguration()));
	connect(m_mainWindowUi->actionToolbarsConfiguration, SIGNAL(triggered()), this, SLOT(actionToolbarsConfiguration()));
	connect(m_mainWindowUi->actionApplicationConfiguration, SIGNAL(triggered()), this, SLOT(actionApplicationConfiguration()));
	connect(m_mainWindowUi->actionAboutApplication, SIGNAL(triggered()), this, SLOT(actionAboutApplication()));
	connect(m_mainWindowUi->actionAboutQt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));
	connect(m_mainWindowUi->menuFile, SIGNAL(aboutToShow()), this, SLOT(updateRecentFilesMenu()));
	connect(m_map2DEditorWidgetUi->map2DViewWidget, SIGNAL(cooridantesChanged(int, int, int)), this, SLOT(updateCoordinates(int, int, int)));
	connect(m_mainWindowUi->map3DViewWidget, SIGNAL(cooridantesChanged(int, int, int)), this, SLOT(updateCoordinates(int, int, int)));
	connect(m_map2DEditorWidgetUi->map2DViewWidget, SIGNAL(zoomChanged(int)), this, SLOT(updateZoom(int)));
	connect(m_mainWindowUi->map3DViewWidget, SIGNAL(zoomChanged(int)), this, SLOT(updateZoom(int)));
	connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(updateZoom(int)));
	connect(m_tilesetUi->tileCategoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));
	connect(m_tilesetUi->tileTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));
	connect(m_tilesetUi->showTransitionTilesCheckBox, SIGNAL(clicked()), this, SLOT(updateTilesetView()));

	m_map2DEditorWidgetUi->map2DViewWidget->setMap(m_map);
	m_mainWindowUi->map3DViewWidget->setMap(m_map);
	m_mainWindowUi->action3DView->setChecked(SettingsManager::getValue("3DView").toBool());

	updateToolBars();
	updateZoom(SettingsManager::getValue("zoomLevel").toInt());
	restoreGeometry(SettingsManager::getValue("geometry").toByteArray());
	restoreState(SettingsManager::getValue("windowState").toByteArray());
	action3DView(m_mainWindowUi->action3DView->isChecked());
	actionLockToolBars(SettingsManager::getValue("lockToolBars").toBool());
}

MainWindow::~MainWindow()
{
	delete m_mainWindowUi;
	delete m_map2DEditorWidgetUi;
	delete m_tilesetUi;
	delete m_terrainUi;
	delete m_informationUi;
	delete m_objectsUi;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (!canClose())
	{
		event->ignore();

		return;
	}

	SettingsManager::setValue("geometry", saveGeometry());
	SettingsManager::setValue("windowState", saveState());

	QMainWindow::closeEvent(event);
}

void MainWindow::actionNew()
{
	if (canClose())
	{
		if (m_map)
		{
			m_map->deleteLater();
		}

		m_map = new Map(this);
		m_map->setModified(true);
		m_map->setTileset(ArizonaTileset);

		m_fileNameLabel->setText("Unnamed");
		m_fileNameLabel->setToolTip(QString());

		setWindowTitle(tr("%1 - Unnamed").arg(ApplicationName));

		m_map2DEditorWidgetUi->map2DViewWidget->setMap(m_map);
		m_mainWindowUi->map3DViewWidget->setMap(m_map);

		actionProperties(true);
	}
}

void MainWindow::actionOpen()
{
	openFile(QFileDialog::getOpenFileName(this, tr("Open map file"), SettingsManager::getValue("lastUsedDir", SettingsManager::getValue("dataPath", QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first()).toString()).toString(), tr("All Compatible (*.gam)")));
}

void MainWindow::actionOpenRecent(QAction *action)
{
	if (!action->data().toString().isEmpty())
	{
		openFile(action->data().toString());
	}
}

void MainWindow::actionClearRecentFiles()
{
	SettingsManager::remove("recentFiles");
}

void MainWindow::actionSave()
{
	QMessageBox::warning(this, tr("Error"), tr("Not implemented yet."));
}

void MainWindow::actionProperties(bool newMap)
{
	new MapSettingsManager(m_map, newMap, this);

	m_tilesetUi->tileCategoryComboBox->clear();

	updateTilesetView();

	m_mainWindowUi->map3DViewWidget->repaint();
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

void MainWindow::action3DView(bool checked)
{
	m_mainWindowUi->map2DEditorWidget->setVisible(!checked);
	m_mainWindowUi->map3DViewWidget->setVisible(checked);

	SettingsManager::setValue("3DView", checked);
}

void MainWindow::actionZoomIn()
{
	updateZoom(m_zoomSlider->value() + 5);
}

void MainWindow::actionZoomOut()
{
	updateZoom(m_zoomSlider->value() - 5);
}

void MainWindow::actionZoomOriginal()
{
	updateZoom(100);
}

void MainWindow::actionShortcutsConfiguration()
{
	new ShortcutsManager(this);
}

void MainWindow::actionToolbarsConfiguration(const QString &toolBar)
{
	new ToolBarsManager(findChildren<ToolBarWidget*>(), toolBar, this);
}

void MainWindow::actionApplicationConfiguration()
{
	new PreferencesManager(this);

	if (Tileset::cachedTextureSize() != SettingsManager::getValue("tileSize").toInt())
	{
		Tileset::createCache(m_map->getTileset(), SettingsManager::getValue("tileSize").toInt());
	}
}

void MainWindow::actionAboutApplication()
{
	QMessageBox::about(this, tr("About Warzone 2100 Map Editor"), QString(tr("<b>%1 %2</b><br>Map viewer and editor for Warzone 2100.").arg(ApplicationName).arg(QApplication::applicationVersion())));
}

void MainWindow::actionLockToolBars(bool lock)
{
	const QList<ToolBarWidget*> toolBars = findChildren<ToolBarWidget*>();

	for (int i = 0; i < toolBars.count(); ++i)
	{
		toolBars.at(i)->setMovable(!lock);
	}

	SettingsManager::setValue("lockToolBars", lock);
}

void MainWindow::actionToggleDock()
{
	QAction *action = qobject_cast<QAction*>(sender());

	if (!action)
	{
		return;
	}

	const QString name = action->objectName().mid(6);

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

void MainWindow::updateRecentFilesMenu()
{
	const QStringList recentFiles = SettingsManager::getValue("recentFiles").toStringList();

	for (int i = 0; i < 10; ++i)
	{
		if (i < recentFiles.count())
		{
			QFileInfo fileInfo(recentFiles.at(i));

			m_mainWindowUi->menuOpenRecent->actions().at(i)->setText(QString("%1. %2 (%3)").arg(i + 1).arg(fileInfo.fileName()).arg(recentFiles.at(i)));
			m_mainWindowUi->menuOpenRecent->actions().at(i)->setData(recentFiles.at(i));
			m_mainWindowUi->menuOpenRecent->actions().at(i)->setVisible(true);
		}
		else
		{
			m_mainWindowUi->menuOpenRecent->actions().at(i)->setVisible(false);
		}
	}

	m_mainWindowUi->menuOpenRecent->setEnabled(recentFiles.count());
}

void MainWindow::updateToolBars()
{
	QStringList currentToolBars = SettingsManager::getValue("toolBars").toStringList();
	const QList<ToolBarWidget*> existingToolBars = findChildren<ToolBarWidget*>();

	for (int i = (existingToolBars.count() - 1); i >= 0; --i)
	{
		if (currentToolBars.contains(existingToolBars.at(i)->objectName()))
		{
			existingToolBars.at(i)->reload();

			currentToolBars.removeAt(i);
		}
		else
		{
			existingToolBars.at(i)->deleteLater();
		}
	}

	for (int i = 0; i < currentToolBars.count(); ++i)
	{
		ToolBarWidget *toolBar = new ToolBarWidget(currentToolBars.at(i), this);
		QAction *action = m_mainWindowUi->menuToolbars->addAction(toolBar->windowTitle());
		action->setCheckable(true);
		action->setChecked(toolBar->isVisible());
		action->setObjectName("toggleToolBar" + currentToolBars.at(i));

		ActionsManager::registerAction(action);

		addToolBar(toolBar);

		connect(action, SIGNAL(toggled(bool)), toolBar, SLOT(setVisible(bool)));
		connect(toolBar, SIGNAL(visibilityChanged(bool)), action, SLOT(setChecked(bool)));
		connect(toolBar, SIGNAL(requestToolBarsLock(bool)), this, SLOT(actionLockToolBars(bool)));
		connect(toolBar, SIGNAL(requestConfigure(QString)), this, SLOT(actionToolbarsConfiguration(QString)));
	}
}

void MainWindow::updateTilesetView()
{
	Tileset *tileset = Tileset::tileset(m_map ? m_map->getTileset() : ArizonaTileset);

	if (m_currentTileset != tileset->type())
	{
		Tileset::createCache(tileset->type(), SettingsManager::getValue("tileSize").toInt());

		m_currentTileset = tileset->type();

		disconnect(m_tilesetUi->tileCategoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));
		disconnect(m_tilesetUi->tileTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));

		m_tilesetUi->tileCategoryComboBox->clear();
		m_tilesetUi->tileCategoryComboBox->addItems(tileset->categories());

		connect(m_tilesetUi->tileCategoryComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));
		connect(m_tilesetUi->tileTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTilesetView()));
	}

	m_tilesetUi->listWidget->clear();

	const QList<TileInformation> tiles = tileset->tiles(m_tilesetUi->showTransitionTilesCheckBox->isChecked(), m_tilesetUi->tileCategoryComboBox->currentIndex(), static_cast<TileTypes>(m_tilesetUi->tileTypeComboBox->itemData(m_tilesetUi->tileTypeComboBox->currentIndex()).toInt()));

	for (int i = 0; i < tiles.count(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(tileset->pixmap(tiles.at(i), 128), QString(), m_tilesetUi->listWidget);
		item->setToolTip(QString("Tile ID: %1").arg(tiles.at(i).id));

		m_tilesetUi->listWidget->addItem(item);
	}
}

void MainWindow::updateCoordinates(int x, int y, int z)
{
	m_coordinatesLabel->setText(QString("X: %1 Y: %2 Z: %3").arg(x).arg(y).arg(z));
}

void MainWindow::updateZoom(int zoom)
{
	if (m_zoomSlider->value() != zoom)
	{
		m_zoomSlider->setValue(zoom);
	}

	if (m_map2DEditorWidgetUi->map2DViewWidget->getZoom() != zoom)
	{
		m_map2DEditorWidgetUi->map2DViewWidget->setZoom(zoom);
	}

	if (m_mainWindowUi->map3DViewWidget->getZoom() != zoom)
	{
		m_mainWindowUi->map3DViewWidget->setZoom(zoom);
	}

	m_zoomSlider->setToolTip(QString("Zoom: %1%").arg(zoom));

	SettingsManager::setValue("zoomLevel", zoom);
}

bool MainWindow::openFile(const QString &fileName)
{
	const QFileInfo fileInfo(fileName);

	if (fileInfo.exists())
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

		MapParser *mapParser = new MapParser(fileName, this);

		if (mapParser->getErrorString().isEmpty())
		{
			m_map2DEditorWidgetUi->map2DViewWidget->setMap(mapParser->getMap());
			m_mainWindowUi->map3DViewWidget->setMap(mapParser->getMap());

			QApplication::restoreOverrideCursor();

			m_map->deleteLater();
			m_map = mapParser->getMap();

			mapParser->deleteLater();

			setWindowTitle(tr("%1 - %2").arg(ApplicationName).arg(fileInfo.fileName()));

			m_fileNameLabel->setText(fileInfo.fileName());
			m_fileNameLabel->setToolTip(fileInfo.absoluteFilePath());

			QStringList recentFiles = SettingsManager::getValue("recentFiles").toStringList();
			recentFiles.removeAll(fileInfo.absoluteFilePath());
			recentFiles.prepend(fileInfo.absoluteFilePath());
			recentFiles = recentFiles.mid(0, 10);

			SettingsManager::setValue("recentFiles", recentFiles);
			SettingsManager::setValue("lastUsedDir", fileInfo.absoluteDir().path());

			if (Tileset::cachedTileset() != m_map->getTileset())
			{
				Tileset::createCache(m_map->getTileset(), SettingsManager::getValue("tileSize").toInt());
			}

			updateTilesetView();

			return true;
		}
		else
		{
			QApplication::restoreOverrideCursor();

			QMessageBox::critical(this, tr("Map loading error"), mapParser->getErrorString());

			mapParser->deleteLater();

			return false;
		}
	}
	else
	{
		return false;
	}
}

bool MainWindow::canClose()
{
	if (isWindowModified())
	{
		QMessageBox::StandardButton action = QMessageBox::warning(QApplication::topLevelWidgets().at(0), ApplicationName, tr("There are unsaved changes in current map.\nDo you want to save your changes?"), (QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel));

		if (action == QMessageBox::Cancel)
		{
			return false;
		}

		if (action == QMessageBox::Save)
		{
			actionSave();
		}
	}

	return true;
}

}
