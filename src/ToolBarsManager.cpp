#include "ToolBarsManager.h"
#include "ToolBarWidget.h"
#include "ActionsManager.h"
#include "SettingsManager.h"

#include "ui_ToolBarEditorDialog.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QApplication>

namespace WZMapEditor
{

ToolBarsManager::ToolBarsManager(QList<ToolBarWidget*> toolBars, ToolBarWidget *configureToolBar, QObject *parent) : QObject(parent),
	m_managerUi(new Ui::ToolBarEditorDialog()),
	m_toolBars(toolBars),
	m_currentToolBar(-1),
	m_isCurrentModified(false)
{
	QDialog managerDialog(QApplication::topLevelWidgets().at(0));
	m_managerUi->setupUi(&managerDialog);

	for (int i = 0; i < toolBars.count(); ++i)
	{
		m_managerUi->toolBarComboBox->addItem(toolBars.at(i)->windowTitle());
	}

	connect(&managerDialog, SIGNAL(finished(int)), this, SLOT(deleteLater()));
	connect(m_managerUi->toolBarComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(loadToolBar(int)));
	connect(m_managerUi->showCheckBox, SIGNAL(clicked()), this, SLOT(setModified()));
	connect(m_managerUi->availableActionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(availableActionsCurrentItemChanged(int)));
	connect(m_managerUi->currentActionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentActionsCurrentItemChanged(int)));
	connect(m_managerUi->removeButton, SIGNAL(clicked()), this, SLOT(removeItem()));
	connect(m_managerUi->addButton, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(m_managerUi->moveUpButton, SIGNAL(clicked()), this, SLOT(moveUpItem()));
	connect(m_managerUi->moveDownButton, SIGNAL(clicked()), this, SLOT(moveDownItem()));
	connect(m_managerUi->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(dialogButtonCliked(QAbstractButton*)));

	int toolBarToLoad = toolBars.indexOf(configureToolBar);

	if (toolBarToLoad < 0)
	{
		toolBarToLoad = 0;
	}

	loadToolBar(toolBarToLoad);

	managerDialog.exec();
}

ToolBarsManager::~ToolBarsManager()
{
	delete m_managerUi;
}

void ToolBarsManager::loadToolBar(int index)
{
	if (index == m_currentToolBar)
	{
		return;
	}

	if (index >= m_toolBars.count())
	{
		m_managerUi->toolBarComboBox->setCurrentIndex(m_currentToolBar);
	}

	if (m_isCurrentModified && index >= 0)
	{
		if (QMessageBox::question(QApplication::topLevelWidgets().at(0), QApplication::applicationName(), tr("There are unsaved changes in this toolbar, do yo want to save them?"), QMessageBox::Save, QMessageBox::Cancel) == QMessageBox::Cancel)
		{
			m_managerUi->toolBarComboBox->setCurrentIndex(m_currentToolBar);

			return;
		}

		saveToolBar();
	}

	m_managerUi->currentActionsListWidget->clear();
	m_managerUi->availableActionsListWidget->clear();
	m_managerUi->availableActionsListWidget->addItem(tr("--- separator ---"));

	const QList<QAction*> currentActions = m_toolBars.at(index)->actions();
	QStringList availableActions = ActionsManager::getActions();
	availableActions.sort();

	for (int i = 0; i < availableActions.count(); ++i)
	{
		QAction *action = ActionsManager::getAction(availableActions.at(i));

		if (!action || action->isSeparator() || currentActions.contains(action))
		{
			continue;
		}

		QListWidgetItem *item = new QListWidgetItem(action->icon(), action->text(), m_managerUi->availableActionsListWidget);
		item->setData(Qt::UserRole, action->objectName());

		m_managerUi->availableActionsListWidget->addItem(item);
	}

	for (int i = 0; i < currentActions.count(); ++i)
	{
		if (currentActions.at(i)->isSeparator())
		{
			m_managerUi->currentActionsListWidget->addItem(new QListWidgetItem(tr("--- separator ---"), m_managerUi->currentActionsListWidget));
		}
		else
		{
			QListWidgetItem *item = new QListWidgetItem(currentActions.at(i)->icon(), currentActions.at(i)->text(), m_managerUi->currentActionsListWidget);
			item->setData(Qt::UserRole, currentActions.at(i)->objectName());

			m_managerUi->currentActionsListWidget->addItem(item);
		}
	}

	m_managerUi->showCheckBox->setChecked(m_toolBars.at(index)->isVisible());
	m_managerUi->toolBarComboBox->setCurrentIndex(index);

	setModified(false);

	m_currentToolBar = index;
}

void ToolBarsManager::availableActionsCurrentItemChanged(int index)
{
	m_managerUi->addButton->setEnabled(index >= 0);

	if (index >= 0)
	{
		m_managerUi->descriptionLabel->setText(m_managerUi->availableActionsListWidget->currentItem()->text());
	}
}

void ToolBarsManager::currentActionsCurrentItemChanged(int index)
{
	m_managerUi->removeButton->setEnabled(index >= 0);
	m_managerUi->moveUpButton->setEnabled(index > 0);
	m_managerUi->moveDownButton->setEnabled(index >= 0 && index < (m_managerUi->currentActionsListWidget->count() - 1));

	if (index >= 0)
	{
		m_managerUi->descriptionLabel->setText(m_managerUi->currentActionsListWidget->currentItem()->text());
	}
}

void ToolBarsManager::removeItem()
{
	if (m_managerUi->currentActionsListWidget->currentRow() >= 0)
	{
		QListWidgetItem *currentItem = m_managerUi->currentActionsListWidget->takeItem(m_managerUi->currentActionsListWidget->currentRow());

		if (currentItem->text() != tr("--- separator ---"))
		{
			m_managerUi->availableActionsListWidget->addItem(currentItem);
		}
		else
		{
			delete currentItem;
		}

		m_managerUi->currentActionsListWidget->setCurrentItem(NULL);
		m_managerUi->availableActionsListWidget->setCurrentItem(NULL);

		setModified(true);
	}
}

void ToolBarsManager::addItem()
{
	if (m_managerUi->availableActionsListWidget->currentRow() >= 0)
	{
		QListWidgetItem *currentItem = m_managerUi->availableActionsListWidget->takeItem(m_managerUi->availableActionsListWidget->currentRow());

		if (currentItem->text() == tr("--- separator ---"))
		{
			m_managerUi->availableActionsListWidget->insertItem(0, currentItem->clone());
		}

		m_managerUi->currentActionsListWidget->insertItem((m_managerUi->currentActionsListWidget->currentRow() + 1), currentItem);
		m_managerUi->currentActionsListWidget->setCurrentItem(currentItem);
		m_managerUi->availableActionsListWidget->setCurrentItem(NULL);

		setModified(true);
	}
}

void ToolBarsManager::moveUpItem()
{
	const int currentRow = m_managerUi->currentActionsListWidget->currentRow();

	if (currentRow > 0)
	{
		QListWidgetItem *currentItem = m_managerUi->currentActionsListWidget->takeItem(currentRow);

		m_managerUi->currentActionsListWidget->insertItem((currentRow - 1), currentItem);
		m_managerUi->currentActionsListWidget->setCurrentItem(currentItem);

		setModified(true);
	}
}

void ToolBarsManager::moveDownItem()
{
	const int currentRow = m_managerUi->currentActionsListWidget->currentRow();

	if (currentRow < (m_managerUi->currentActionsListWidget->count() - 1))
	{
		QListWidgetItem *currentItem = m_managerUi->currentActionsListWidget->takeItem(currentRow);

		m_managerUi->currentActionsListWidget->insertItem((currentRow + 1), currentItem);
		m_managerUi->currentActionsListWidget->setCurrentItem(currentItem);

		setModified(true);
	}
}

void ToolBarsManager::dialogButtonCliked(QAbstractButton *button)
{
	switch (m_managerUi->buttonBox->standardButton(button))
	{
		case QDialogButtonBox::Ok:
			saveToolBar();
			deleteLater();
		break;
		case QDialogButtonBox::Apply:
			saveToolBar();
		break;
		case QDialogButtonBox::RestoreDefaults:
			restoreToolBar();
		break;
		default:
			deleteLater();
		break;
	}
}

void ToolBarsManager::saveToolBar()
{
	QStringList actions;

	for (int i = 0; i < m_managerUi->currentActionsListWidget->count(); ++i)
	{
		actions.append(m_managerUi->currentActionsListWidget->item(i)->data(Qt::UserRole).toString());
	}

	SettingsManager::setValue("toolbars/" + m_toolBars.at(m_currentToolBar)->objectName(), actions);

	m_toolBars.at(m_currentToolBar)->setVisible(m_managerUi->showCheckBox->isChecked());
	m_toolBars.at(m_currentToolBar)->reload();

	setModified(false);
}

void ToolBarsManager::restoreToolBar()
{
	SettingsManager::restore("toolbars/" + m_toolBars.at(m_currentToolBar)->objectName());

	m_toolBars.at(m_currentToolBar)->setVisible(true);
	m_toolBars.at(m_currentToolBar)->reload();

	setModified(false);

	int currentToolBar = m_currentToolBar;

	m_currentToolBar = -1;

	loadToolBar(currentToolBar);
}

void ToolBarsManager::setModified(bool modified)
{
	m_managerUi->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(modified);

	m_isCurrentModified = modified;
}

}
