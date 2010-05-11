#include "ShortcutManager.h"
#include "ShortcutEditorDelegate.h"
#include "ActionManager.h"
#include "MainWindow.h"

#include "ui_ShortcutEditorDialog.h"

#include <QtGui/QMessageBox>
#include <QtGui/QApplication>
#include <QtGui/QTableWidgetItem>

namespace WZMapEditor
{

ShortcutManager::ShortcutManager(MainWindow *parent) : QObject(parent),
	m_managerUi(new Ui::ShortcutEditorDialog()),
	m_mainWindow(parent)
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);

	m_managerUi->shortcutsView->setColumnCount(2);
	m_managerUi->shortcutsView->setRowCount(ActionManager::actions().count());
	m_managerUi->shortcutsView->setItemDelegate(new ShortcutEditorDelegate(this));

	for (int i = 0; i < ActionManager::actions().count(); ++i)
	{
		QTableWidgetItem *descriptionItem = new QTableWidgetItem(ActionManager::actions().at(i)->icon(), ActionManager::actions().at(i)->text());
		descriptionItem->setToolTip(ActionManager::actions().at(i)->text());
		descriptionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		QTableWidgetItem *editingItem = new QTableWidgetItem(ActionManager::actions().at(i)->shortcut().toString(QKeySequence::NativeText));
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
	for (int i = 0; i < ActionManager::actions().count(); ++i)
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
	for (int i = 0; i < ActionManager::actions().count(); ++i)
	{
		ActionManager::setShortcut(ActionManager::actions().at(i), QKeySequence(m_managerUi->shortcutsView->item(i, 1)->text()));
	}
}

void ShortcutManager::dialogButtonCliked(QAbstractButton *button)
{
	if (m_managerUi->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults)
	{
		for (int i = 0; i < ActionManager::actions().count(); ++i)
		{
			ActionManager::restoreDefaultShortcut(ActionManager::actions().at(i));

			m_managerUi->shortcutsView->item(i, 1)->setText(ActionManager::actions().at(i)->shortcut().toString(QKeySequence::NativeText));
		}
	}
	else
	{
		deleteLater();
	}
}

QString ShortcutManager::restoreDefaultShortcut(int index)
{
	if (index >= 0 && index < ActionManager::actions().count())
	{
		ActionManager::restoreDefaultShortcut(ActionManager::actions().at(index));

		return ActionManager::actions().at(index)->shortcut().toString(QKeySequence::NativeText);
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
