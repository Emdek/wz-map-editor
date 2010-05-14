#include "PreferencesManager.h"
#include "SettingManager.h"
#include "FileSystemCompleterModel.h"

#include "ui_PreferencesDialog.h"

#include <QtGui/QApplication>
#include <QtGui/QCompleter>
#include <QtGui/QFileDialog>


namespace WZMapEditor
{

PreferencesManager::PreferencesManager(QObject *parent) : QObject(parent),
	m_managerUi(new Ui::PreferencesDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);
	m_managerUi->dataPathLineEdit->setCompleter(new QCompleter(new FileSystemCompleterModel(this), this));
	m_managerUi->dataPathLineEdit->setText(SettingManager::value("dataPath").toString());

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
