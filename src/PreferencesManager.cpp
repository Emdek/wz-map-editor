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
	m_managerUi->texturesPathLineEdit->setCompleter(new QCompleter(new FileSystemCompleterModel(this), this));
	m_managerUi->texturesPathLineEdit->setText(SettingManager::value("texturesPath").toString());

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_managerUi->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));
	connect(m_managerUi->selectTexturesPathButton, SIGNAL(clicked()), this, SLOT(selectTexturesPath()));

	managerDialog.exec();
}

PreferencesManager::~PreferencesManager()
{
	delete m_managerUi;
}

void PreferencesManager::selectTexturesPath()
{
	m_managerUi->texturesPathLineEdit->setText(QFileDialog::getExistingDirectory(QApplication::topLevelWidgets().at(0), tr("Select textures path"), m_managerUi->texturesPathLineEdit->text()));
}

void PreferencesManager::save()
{
	SettingManager::setValue("texturesPath", m_managerUi->texturesPathLineEdit->text());
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
