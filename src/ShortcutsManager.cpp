#include "ShortcutsManager.h"
#include "ShortcutEditorDelegate.h"
#include "ActionsManager.h"

#include "ui_ShortcutEditorDialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTableWidgetItem>

namespace WZMapEditor
{

ShortcutsManager::ShortcutsManager(QWidget *parent) : QObject(parent),
	m_ui(new Ui::ShortcutEditorDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_ui->setupUi(&managerDialog);

	m_ui->shortcutsView->setColumnCount(2);
	m_ui->shortcutsView->setRowCount(ActionsManager::getIdentifiers().count());
	m_ui->shortcutsView->setItemDelegate(new ShortcutEditorDelegate(this));

	const QStringList actions = ActionsManager::getIdentifiers();

	for (int i = 0; i < actions.count(); ++i)
	{
		QAction *action = ActionsManager::getAction(actions.at(i));
		QTableWidgetItem *descriptionItem = new QTableWidgetItem(action->icon(), action->text());
		descriptionItem->setToolTip(action->text());
		descriptionItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		QTableWidgetItem *editingItem = new QTableWidgetItem(action->shortcut().toString(QKeySequence::NativeText));
		editingItem->setData(Qt::UserRole, action->objectName());
		editingItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled);

		m_ui->shortcutsView->setItem(i, 0, descriptionItem);
		m_ui->shortcutsView->setItem(i, 1, editingItem);
	}

	m_ui->shortcutsView->resizeColumnsToContents();
	m_ui->shortcutsView->horizontalHeader()->setStretchLastSection(true);

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(&managerDialog, SIGNAL(accepted()), this, SLOT(save()));
	connect(m_ui->filterLineEdit, SIGNAL(textChanged(QString)), this, SLOT(filter(QString)));
	connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));

	managerDialog.exec();
}

ShortcutsManager::~ShortcutsManager()
{
	delete m_ui;
}

void ShortcutsManager::filter(const QString &filter)
{
	for (int i = 0; i < ActionsManager::getIdentifiers().count(); ++i)
	{
		if (filter.isEmpty() || m_ui->shortcutsView->item(i, 0)->text().contains(filter, Qt::CaseInsensitive) || m_ui->shortcutsView->item(i, 1)->text().contains(filter, Qt::CaseInsensitive))
		{
			m_ui->shortcutsView->setRowHidden(i, false);
		}
		else
		{
			m_ui->shortcutsView->setRowHidden(i, true);
		}
	}
}

void ShortcutsManager::save()
{
	const QStringList actions = ActionsManager::getIdentifiers();

	for (int i = 0; i < actions.count(); ++i)
	{
		ActionsManager::setShortcut(ActionsManager::getAction(actions.at(i)), QKeySequence(m_ui->shortcutsView->item(i, 1)->text()));
	}
}

void ShortcutsManager::dialogButtonCliked(QAbstractButton *button)
{
	if (m_ui->buttonBox->standardButton(button) != QDialogButtonBox::RestoreDefaults)
	{
		deleteLater();

		return;
	}

	const QStringList actions = ActionsManager::getIdentifiers();

	for (int i = 0; i < actions.count(); ++i)
	{
		QAction *action = ActionsManager::getAction(actions.at(i));

		ActionsManager::restoreDefaultShortcut(action);

		m_ui->shortcutsView->item(i, 1)->setText(action->shortcut().toString(QKeySequence::NativeText));
	}
}

bool ShortcutsManager::checkShortcut(const QKeySequence &shortcut, int index)
{
	if (shortcut.isEmpty())
	{
		return true;
	}

	for (int i = 0; i < m_ui->shortcutsView->rowCount(); ++i)
	{
		if (!m_ui->shortcutsView->item(i, 1)->text().isEmpty() && i != index && QKeySequence(m_ui->shortcutsView->item(i, 1)->text()).matches(shortcut) == QKeySequence::ExactMatch)
		{
			if (QMessageBox::question(QApplication::topLevelWidgets().at(0), QApplication::applicationName(), tr("Shortcut \"%1\" is already used by action \"%2\".\nDo you want to replace it?").arg(shortcut.toString(QKeySequence::NativeText)).arg(m_ui->shortcutsView->item(i, 0)->text()), QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok)
			{
				m_ui->shortcutsView->item(i, 1)->setText(QString());

				return true;
			}

			return false;
		}
	}

	return true;
}

}
