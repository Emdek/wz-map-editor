#include "PreferencesManager.h"
#include "SettingsManager.h"
#include "FileSystemCompleterModel.h"

#include "ui_PreferencesDialog.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QFileDialog>

#include <math.h>

namespace WZMapEditor
{

PreferencesManager::PreferencesManager(QObject *parent) : QObject(parent),
	m_ui(new Ui::PreferencesDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	int tileSize = 0;

	switch (SettingsManager::getValue("tileSize").toInt())
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

	m_ui->setupUi(&managerDialog);
	m_ui->dataPathLineEdit->setCompleter(new QCompleter(new FileSystemCompleterModel(this), this));
	m_ui->dataPathLineEdit->setText(SettingsManager::getValue("dataPath").toString());
	m_ui->tileSizeComboBox->setCurrentIndex(tileSize);

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));
	connect(m_ui->selectDataPathButton, SIGNAL(clicked()), this, SLOT(selectDataPath()));

	managerDialog.exec();
}

PreferencesManager::~PreferencesManager()
{
	delete m_ui;
}

void PreferencesManager::selectDataPath()
{
	m_ui->dataPathLineEdit->setText(QFileDialog::getExistingDirectory(QApplication::topLevelWidgets().at(0), tr("Select Data Path"), m_ui->dataPathLineEdit->text()));
}

void PreferencesManager::save()
{
	SettingsManager::setValue("dataPath", m_ui->dataPathLineEdit->text());
	SettingsManager::setValue("tileSize", (pow(2, (3 - m_ui->tileSizeComboBox->currentIndex())) * 16));
}

void PreferencesManager::dialogButtonCliked(QAbstractButton *button)
{
	if (m_ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply)
	{
		save();
	}
	else
	{
		deleteLater();
	}
}

}
