#include "ShortcutManager.h"
#include "ShortcutEditorDelegate.h"
#include "MainWindow.h"

#include "ui_ShortcutEditorDialog.h"

#include <QtCore/QSettings>

#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QTableWidgetItem>

namespace WZMapEditor
{

ShortcutManager::ShortcutManager(QList<QAction*> actions, MainWindow *parent) : QObject(parent),
	m_managerUi(new Ui::ShortcutEditorDialog()),
	m_mainWindow(parent),
	m_actions(actions)
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);

	m_managerUi->shortcutsView->setColumnCount(2);
	m_managerUi->shortcutsView->setRowCount(actions.count());
	m_managerUi->shortcutsView->setItemDelegate(new ShortcutEditorDelegate(this));

	for (int i = 0; i < actions.count(); ++i)
	{
		QTableWidgetItem *descriptionItem = new QTableWidgetItem(actions.at(i)->icon(), actions.at(i)->text());
		descriptionItem->setToolTip(actions.at(i)->text());
		descriptionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		QTableWidgetItem *editingItem = new QTableWidgetItem(actions.at(i)->shortcut().toString(QKeySequence::NativeText));
		editingItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		m_managerUi->shortcutsView->setItem(i, 0, descriptionItem);
		m_managerUi->shortcutsView->setItem(i, 1, editingItem);
	}

	m_managerUi->shortcutsView->resizeColumnsToContents();
	m_managerUi->shortcutsView->horizontalHeader()->setStretchLastSection(true);

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_managerUi->filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));
	connect(m_managerUi->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));

	managerDialog.exec();
}

ShortcutManager::~ShortcutManager()
{
	delete m_managerUi;
}

void ShortcutManager::filter(const QString &filter)
{
	for (int i = 0; i < m_actions.count(); ++i)
	{
		if (filter.isEmpty() || m_managerUi->shortcutsView->item(i, 0)->text().contains(filter, Qt::CaseInsensitive) || m_managerUi->shortcutsView->item(i, 1)->text().contains(filter, Qt::CaseInsensitive))
		{
			m_managerUi->shortcutsView->setRowHidden(i, false);
		}
		else
		{
			m_managerUi->shortcutsView->setRowHidden(i, true);
		}
	}
}

void ShortcutManager::save()
{
	for (int i = 0; i < m_actions.count(); ++i)
	{
		m_actions.at(i)->setShortcut(QKeySequence(m_managerUi->shortcutsView->item(i, 1)->text()));

		QSettings().setValue(QString("Shortcuts/%1").arg(m_actions.at(i)->objectName()), m_actions.at(i)->shortcut().toString());
	}
}

void ShortcutManager::dialogButtonCliked(QAbstractButton *button)
{
	if (m_managerUi->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults)
	{
		for (int i = 0; i < m_actions.count(); ++i)
		{
//			m_mainWindow->restoreDefaultShortcut(m_actions.at(i));

			m_managerUi->shortcutsView->item(i, 1)->setText(m_actions.at(i)->shortcut().toString(QKeySequence::NativeText));
		}
	}
	else
	{
		if (m_managerUi->buttonBox->standardButton(button) == QDialogButtonBox::Cancel)
		{
//			m_mainWindow->loadShortcuts();
		}

		deleteLater();
	}
}

QString ShortcutManager::restoreDefaultShortcut(int index)
{
	if (index >= 0 && index < m_actions.count())
	{
//		m_mainWindow->restoreDefaultShortcut(m_actions.at(index));

		return m_actions.at(index)->shortcut().toString(QKeySequence::NativeText);
	}

	return QString();
}

bool ShortcutManager::checkSequence(const QKeySequence &sequence, int index)
{
	if (sequence.isEmpty())
	{
		return true;
	}

	for (int i = 0; i < m_managerUi->shortcutsView->rowCount(); ++i)
	{
		if (!m_managerUi->shortcutsView->item(i, 1)->text().isEmpty() && i != index && QKeySequence(m_managerUi->shortcutsView->item(i, 1)->text()).matches(sequence) == QKeySequence::ExactMatch)
		{
			if (QMessageBox::question(QApplication::topLevelWidgets().at(0), QApplication::applicationName(), tr("Shortcut \"%1\" is already used by action \"%2\".\nDo you want to replace it?").arg(sequence.toString(QKeySequence::NativeText)).arg(m_managerUi->shortcutsView->item(i, 0)->text()), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
			{
				m_managerUi->shortcutsView->item(i, 1)->setText(QString());

				return true;
			}

			return false;
		}
	}

	return true;
}

}
