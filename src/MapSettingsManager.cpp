#include "MapSettingsManager.h"
#include "Map.h"
#include "Tileset.h"

#include "ui_MapSettingsDialog.h"

#include <QtCore/QTime>
#include <QtWidgets/QMessageBox>

namespace WZMapEditor
{

MapSettingsManager::MapSettingsManager(Map *map, bool newMap, QObject *parent) : QObject(parent),
	m_map(map),
	m_newMap(newMap),
	m_ui(new Ui::MapSettingsDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_ui->setupUi(&managerDialog);
	m_ui->nameLineEdit->setText(m_map->getName());
	m_ui->tilesetComboBox->addItems(Tileset::getNames());
	m_ui->tilesetComboBox->setCurrentIndex(static_cast<int>(m_map->getTileset()) - 1);
	m_ui->widthSpinBox->setValue(m_map->getSize().width());
	m_ui->heightSpinBox->setValue(m_map->getSize().height());
	m_ui->timeEdit->setTime(QTime().addSecs(m_map->getTime()));
	m_ui->minimumXSpinBox->setValue(m_map->getScrollLimits().left());
	m_ui->maximumXSpinBox->setValue(m_map->getScrollLimits().right());
	m_ui->minimumYSpinBox->setValue(m_map->getScrollLimits().bottom());
	m_ui->maximumYSpinBox->setValue(m_map->getScrollLimits().top());

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_ui->tilesetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTileset(int)));

	managerDialog.exec();
}

MapSettingsManager::~MapSettingsManager()
{
	delete m_ui;
}

void MapSettingsManager::setTileset(int index)
{
	if (!m_newMap && (index != (static_cast<int>(m_map->getTileset()) - 1) && QMessageBox::question(QApplication::topLevelWidgets().at(0), tr("Confirm"), tr("Do you really want to change tileset?\nThis change require reset of map information!"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No))
	{
		m_ui->tilesetComboBox->setCurrentIndex(static_cast<int>(m_map->getTileset()) - 1);
	}
}

void MapSettingsManager::save()
{
	QRect scrollLimits;
	scrollLimits.setLeft(m_ui->minimumXSpinBox->value());
	scrollLimits.setRight(m_ui->maximumXSpinBox->value());
	scrollLimits.setTop(m_ui->minimumYSpinBox->value());
	scrollLimits.setBottom(m_ui->maximumYSpinBox->value());

	if (m_ui->tilesetComboBox->currentIndex() != (static_cast<int>(m_map->getTileset()) - 1))
	{
		m_map->clear();
	}

	m_map->setName(m_ui->nameLineEdit->text());
	m_map->setSize(QSize(m_ui->widthSpinBox->value(), m_ui->heightSpinBox->value()));
	m_map->setScrollLimits(scrollLimits);
	m_map->setTileset(static_cast<TilesetType>(m_ui->tilesetComboBox->currentIndex() + 1));
}

}
