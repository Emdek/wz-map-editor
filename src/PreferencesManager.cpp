#include "PreferencesManager.h"
#include "SettingManager.h"
#include "FileSystemCompleterModel.h"

#include "ui_PreferencesDialog.h"

#include <QtGui/QApplication>
#include <QtGui/QCompleter>
#include <QtGui/QFileDialog>

#include <math.h>


namespace WZMapEditor
{

PreferencesManager::PreferencesManager(QObject *parent) : QObject(parent),
	m_managerUi(new Ui::PreferencesDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	int tileSize = 0;

	switch (SettingManager::value("tileSize").toInt())
	{
		case 16:
			tileSize = 3;
			break;
		case 32:
			tileSize = 2;
			break;
		case 64:
			tileSize = 1;
			break;
		default:
			tileSize = 0;
			break;
	}

	m_managerUi->setupUi(&managerDialog);
	m_managerUi->dataPathLineEdit->setCompleter(new QCompleter(new FileSystemCompleterModel(this), this));
	m_managerUi->dataPathLineEdit->setText(SettingManager::value("dataPath").toString());
	m_managerUi->tileSizeComboBox->setCurrentIndex(tileSize);

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_managerUi->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));
	connect(m_managerUi->selectDataPathButton, SIGNAL(clicked()), this, SLOT(selectDataPath()));

	managerDialog.exec();
}

PreferencesManager::~PreferencesManager()
{
	delete m_managerUi;
}

void PreferencesManager::selectDataPath()
{
	m_managerUi->dataPathLineEdit->setText(QFileDialog::getExistingDirectory(QApplication::topLevelWidgets().at(0), tr("Select Data Path"), m_managerUi->dataPathLineEdit->text()));
}

void PreferencesManager::save()
{
	SettingManager::setValue("dataPath", m_managerUi->dataPathLineEdit->text());
	SettingManager::setValue("tileSize", (pow(2, (3 - m_managerUi->tileSizeComboBox->currentIndex())) * 16));
}

void PreferencesManager::dialogButtonCliked(QAbstractButton *button)
{
	if (m_managerUi->buttonBox->standardButton(button) == QDialogButtonBox::Apply)
	{
		save();
	}
	else
	{
		deleteLater();
	}
}

}
