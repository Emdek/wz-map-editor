#include "ToolBarsManager.h"
#include "ToolBarWidget.h"
#include "ActionsManager.h"
#include "SettingsManager.h"

#include "ui_ToolBarEditorDialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QInputDialog>

namespace WZMapEditor
{

ToolBarsManager::ToolBarsManager(const QString &selectToolBar, QObject *parent) : QObject(parent),
	m_ui(new Ui::ToolBarEditorDialog())
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));

	m_ui->setupUi(&managerDialog);

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(m_ui->toolBarComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadToolBar(int)));
	connect(m_ui->addToolBarButton, SIGNAL(clicked()), this, SLOT(addToolBar()));
	connect(m_ui->removeToolBarButton, SIGNAL(clicked()), this, SLOT(removeToolBar()));
	connect(m_ui->renameToolBarButton, SIGNAL(clicked()), this, SLOT(renameToolBar()));
	connect(m_ui->visibleCheckBox, SIGNAL(clicked()), this, SLOT(setModified()));
	connect(m_ui->availableActionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(availableActionsCurrentItemChanged(int)));
	connect(m_ui->currentActionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentActionsCurrentItemChanged(int)));
	connect(m_ui->removeButton, SIGNAL(clicked()), this, SLOT(removeItem()));
	connect(m_ui->addButton, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(m_ui->moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpItem()));
	connect(m_ui->moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownItem()));
	connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));

	reloadToolbars(selectToolBar);

	managerDialog.exec();
}

ToolBarsManager::~ToolBarsManager()
{
	delete m_ui;
}

void ToolBarsManager::reloadToolbars(const QString &selectToolBar)
{
	const QStringList toolBars = SettingsManager::getValue("toolBars").toStringList();
	int toolBarToLoad = 0;

	for (int i = 0; i < toolBars.count(); ++i)
	{
		QVariantHash hash;
		hash["identifier"] = toolBars.at(i);
		hash["actions"] = SettingsManager::getValue("ToolBars/" + toolBars.at(i) + "/actions").toStringList();

		m_ui->toolBarComboBox->addItem(SettingsManager::getValue("ToolBars/" + toolBars.at(i) + "/title").toString(), hash);

		if (toolBars.at(i) == selectToolBar)
		{
			toolBarToLoad = i;
		}
	}

	loadToolBar(toolBarToLoad);
}

void ToolBarsManager::loadToolBar(int index)
{
	if (index >= m_ui->toolBarComboBox->count())
	{
		m_ui->toolBarComboBox->setCurrentIndex(0);
	}

	m_ui->currentActionsListWidget->clear();
	m_ui->availableActionsListWidget->clear();
	m_ui->availableActionsListWidget->addItem(tr("--- separator ---"));

	const QStringList availableIdentifiers = ActionsManager::getIdentifiers();
	const QStringList currentIdentifiers = m_ui->toolBarComboBox->itemData(index, Qt::UserRole).toHash()["actions"].toStringList();
	QMultiMap<QString, QAction*> actionsMap;

	for (int i = 0; i < availableIdentifiers.count(); ++i)
	{
		QAction *action = ActionsManager::getAction(availableIdentifiers.at(i));

		if (!action || action->isSeparator() || currentIdentifiers.contains(availableIdentifiers.at(i)))
		{
			continue;
		}

		actionsMap.insert(action->text(), action);
	}

	const QList<QAction*> availableActions = actionsMap.values();

	for (int i = 0; i < availableActions.count(); ++i)
	{
		QListWidgetItem *item = new QListWidgetItem(availableActions.at(i)->icon(), availableActions.at(i)->text(), m_ui->availableActionsListWidget);
		item->setData(Qt::UserRole, availableActions.at(i)->objectName());

		m_ui->availableActionsListWidget->addItem(item);
	}

	for (int i = 0; i < currentIdentifiers.count(); ++i)
	{
		QAction *action = ActionsManager::getAction(currentIdentifiers.at(i));

		if (!action || action->isSeparator())
		{
			m_ui->currentActionsListWidget->addItem(new QListWidgetItem(tr("--- separator ---"), m_ui->currentActionsListWidget));
		}
		else
		{
			QListWidgetItem *item = new QListWidgetItem(action->icon(), action->text(), m_ui->currentActionsListWidget);
			item->setData(Qt::UserRole, currentIdentifiers.at(i));

			m_ui->currentActionsListWidget->addItem(item);
		}
	}

//	m_ui->visibleCheckBox->setChecked(m_widgets.at(index)->isVisible());
	m_ui->toolBarComboBox->setCurrentIndex(index);
}

void ToolBarsManager::storeToolBar()
{
	QStringList actions;

	for (int i = 0; i < m_ui->currentActionsListWidget->count(); ++i)
	{
		actions.append(m_ui->currentActionsListWidget->item(i)->data(Qt::UserRole).toString());
	}

	QVariantHash hash = m_ui->toolBarComboBox->itemData(m_ui->toolBarComboBox->currentIndex()).toHash();
	hash["actions"] = actions;

	m_ui->toolBarComboBox->setItemData(m_ui->toolBarComboBox->currentIndex(), hash);
}

void ToolBarsManager::addToolBar()
{
	const QString name = QInputDialog::getText(QApplication::topLevelWidgets().at(0), tr("Select Toolbar Name"), tr("Toolbar Name:"));

	if (name.isEmpty())
	{
		return;
	}

	const QString identifier = QString("toolBar-%1");
	const int index = (m_ui->toolBarComboBox->currentIndex() + 1);
	int i = 1;

	while (m_ui->toolBarComboBox->findData(identifier.arg(i)) >= 0)
	{
		++i;
	}

	m_ui->toolBarComboBox->insertItem(index, name, identifier.arg(i));
	m_ui->toolBarComboBox->setCurrentIndex(index);
	m_ui->visibleCheckBox->setChecked(true);
	m_ui->removeToolBarButton->setEnabled(true);
	m_ui->renameToolBarButton->setEnabled(true);
}

void ToolBarsManager::removeToolBar()
{
	if (QMessageBox::question(QApplication::topLevelWidgets().at(0), tr("Question"), tr("Do you really want to delete toolbar \"%1\"?").arg(m_ui->toolBarComboBox->itemText(m_ui->toolBarComboBox->currentIndex())), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
	{
		return;
	}

	m_ui->toolBarComboBox->removeItem(m_ui->toolBarComboBox->currentIndex());
	m_ui->toolBarComboBox->setCurrentIndex(m_ui->toolBarComboBox->currentIndex());
	m_ui->removeToolBarButton->setEnabled(m_ui->toolBarComboBox->count() > 0);
	m_ui->renameToolBarButton->setEnabled(m_ui->toolBarComboBox->count() > 0);
}

void ToolBarsManager::renameToolBar()
{
	const QString name = QInputDialog::getText(QApplication::topLevelWidgets().at(0), tr("Select Toolbar Name"), tr("Toolbar Name:"), QLineEdit::Normal, m_ui->toolBarComboBox->currentText());

	if (name.isEmpty())
	{
		return;
	}

	m_ui->toolBarComboBox->setItemText(m_ui->toolBarComboBox->currentIndex(), name);
}

void ToolBarsManager::availableActionsCurrentItemChanged(int index)
{
	m_ui->addButton->setEnabled(index >= 0);

	if (index >= 0)
	{
		m_ui->descriptionLabel->setText(m_ui->availableActionsListWidget->currentItem()->text());
	}
}

void ToolBarsManager::currentActionsCurrentItemChanged(int index)
{
	m_ui->removeButton->setEnabled(index >= 0);
	m_ui->moveUpButton->setEnabled(index > 0);
	m_ui->moveDownButton->setEnabled(index >= 0 && index < (m_ui->currentActionsListWidget->count() - 1));

	if (index >= 0)
	{
		m_ui->descriptionLabel->setText(m_ui->currentActionsListWidget->currentItem()->text());
	}
}

void ToolBarsManager::removeItem()
{
	if (m_ui->currentActionsListWidget->currentRow() < 0)
	{
		return;
	}

	QListWidgetItem *currentItem = m_ui->currentActionsListWidget->takeItem(m_ui->currentActionsListWidget->currentRow());

	if (currentItem->text() != tr("--- separator ---"))
	{
		m_ui->availableActionsListWidget->addItem(currentItem);
	}
	else
	{
		delete currentItem;
	}

	m_ui->currentActionsListWidget->setCurrentItem(NULL);
	m_ui->availableActionsListWidget->setCurrentItem(NULL);

	storeToolBar();
}

void ToolBarsManager::addItem()
{
	if (m_ui->availableActionsListWidget->currentRow() < 0)
	{
		return;
	}

	QListWidgetItem *currentItem = m_ui->availableActionsListWidget->takeItem(m_ui->availableActionsListWidget->currentRow());

	if (currentItem->text() == tr("--- separator ---"))
	{
		m_ui->availableActionsListWidget->insertItem(0, currentItem->clone());
	}

	m_ui->currentActionsListWidget->insertItem((m_ui->currentActionsListWidget->currentRow() + 1), currentItem);
	m_ui->currentActionsListWidget->setCurrentItem(currentItem);
	m_ui->availableActionsListWidget->setCurrentItem(NULL);

	storeToolBar();
}

void ToolBarsManager::moveUpItem()
{
	const int currentRow = m_ui->currentActionsListWidget->currentRow();

	if (currentRow > 0)
	{
		QListWidgetItem *currentItem = m_ui->currentActionsListWidget->takeItem(currentRow);

		m_ui->currentActionsListWidget->insertItem((currentRow - 1), currentItem);
		m_ui->currentActionsListWidget->setCurrentItem(currentItem);

		storeToolBar();
	}
}

void ToolBarsManager::moveDownItem()
{
	const int currentRow = m_ui->currentActionsListWidget->currentRow();

	if (currentRow < (m_ui->currentActionsListWidget->count() - 1))
	{
		QListWidgetItem *currentItem = m_ui->currentActionsListWidget->takeItem(currentRow);

		m_ui->currentActionsListWidget->insertItem((currentRow + 1), currentItem);
		m_ui->currentActionsListWidget->setCurrentItem(currentItem);

		storeToolBar();
	}
}

void ToolBarsManager::dialogButtonCliked(QAbstractButton *button)
{
	const QDialogButtonBox::StandardButton standardButton = m_ui->buttonBox->standardButton(button);
	QStringList toolBars;

	switch (standardButton)
	{
		case QDialogButtonBox::Apply:
		case QDialogButtonBox::Ok:
			SettingsManager::restore("toolBars");

			for (int i = 0; i < m_ui->toolBarComboBox->count(); ++i)
			{
				const QVariantHash hash = m_ui->toolBarComboBox->itemData(i).toHash();

				toolBars.append(hash["identifier"].toString());

				SettingsManager::setValue(("ToolBars/" + hash["identifier"].toString() + "/actions"), hash["actions"]);
			}

			SettingsManager::setValue("ToolBars", toolBars);

			if (standardButton == QDialogButtonBox::Ok)
			{
				deleteLater();
			}

			break;
		case QDialogButtonBox::RestoreDefaults:
			SettingsManager::restore("toolBars");
			SettingsManager::restore("ToolBars");

			reloadToolbars();

			break;
		default:
			deleteLater();

			break;
	}
}

}
