#include "MapSettingsManager.h"
#include "MapInformation.h"
#include "Tileset.h"

#include "ui_MapSettingsDialog.h"

#include <QtGui/QMessageBox>


namespace WZMapEditor
{

MapSettingsManager::MapSettingsManager(MapInformation *mapInformation, QObject *parent) : QObject(parent),
	m_mapInformation(mapInformation),
	m_managerUi(new Ui::MapSettingsDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);
	m_managerUi->nameLineEdit->setText(m_mapInformation->name());
	m_managerUi->tilesetComboBox->addItems(Tileset::names());
	m_managerUi->tilesetComboBox->setCurrentIndex(static_cast<int>(m_mapInformation->tilesetType()) - 1);
	m_managerUi->widthSpinBox->setValue(m_mapInformation->size().width());
	m_managerUi->heightSpinBox->setValue(m_mapInformation->size().height());

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_managerUi->tilesetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeTileset(int)));

	managerDialog.exec();
}

MapSettingsManager::~MapSettingsManager()
{
	delete m_managerUi;
}

void MapSettingsManager::changeTileset(int index)
{
	if (index != (static_cast<int>(m_mapInformation->tilesetType()) - 1) && QMessageBox::question(QApplication::topLevelWidgets().at(0), tr("Confirm"), tr("Do you really want to change tileset?\nThis change require reset of map information!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
	{
		m_managerUi->tilesetComboBox->setCurrentIndex(static_cast<int>(m_mapInformation->tilesetType()) - 1);
	}
}

void MapSettingsManager::save()
{
	if (m_managerUi->tilesetComboBox->currentIndex()  != (static_cast<int>(m_mapInformation->tilesetType()) - 1))
	{
		m_mapInformation->clear();
	}

	m_mapInformation->setName(m_managerUi->nameLineEdit->text());
	m_mapInformation->setSize(QSize(m_managerUi->widthSpinBox->value(), m_managerUi->heightSpinBox->value()));
	m_mapInformation->setTilesetType(static_cast<TilesetType>(m_managerUi->tilesetComboBox->currentIndex() + 1));
}

}
