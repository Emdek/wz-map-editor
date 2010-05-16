#include "MapSettingsManager.h"
#include "MapInformation.h"
#include "Tileset.h"

#include "ui_MapSettingsDialog.h"


namespace WZMapEditor
{

MapSettingsManager::MapSettingsManager(MapInformation *mapInformation, QObject *parent) : QObject(parent),
	m_mapInformation(mapInformation),
	m_managerUi(new Ui::MapSettingsDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);
	m_managerUi->tilesetComboBox->addItems(Tileset::names());
	m_managerUi->widthSpinBox->setValue(m_mapInformation->size().width());
	m_managerUi->heightSpinBox->setValue(m_mapInformation->size().height());

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));

	managerDialog.exec();
}

MapSettingsManager::~MapSettingsManager()
{
	delete m_managerUi;
}

void MapSettingsManager::save()
{
	m_mapInformation->setSize(QSize(m_managerUi->widthSpinBox->value(), m_managerUi->heightSpinBox->value()));

///FIXME warn about tileset change
}

}
