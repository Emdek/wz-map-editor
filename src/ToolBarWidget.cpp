#include "ToolBarWidget.h"
#include "ActionManager.h"
#include "SettingManager.h"

#include <QtWidgets/QMenu>

namespace WZMapEditor
{

ToolBarWidget::ToolBarWidget(QMainWindow *parent) : QToolBar(parent)
{
}

void ToolBarWidget::showEvent(QShowEvent *event)
{
	emit visibilityChanged(true);

	QToolBar::showEvent(event);
}

void ToolBarWidget::hideEvent(QHideEvent *event)
{
	emit visibilityChanged(false);

	QToolBar::hideEvent(event);
}

void ToolBarWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
	QAction *actionShow = menu.addAction(tr("Show Toolbar: %1").arg(windowTitle()));
	actionShow->setCheckable(true);
	actionShow->setChecked(true);

	QAction *actionLock = menu.addAction(tr("Lock Toolbars"));
	actionLock->setCheckable(true);
	actionLock->setChecked(!isMovable());

	QAction *actionConfigure = menu.addAction(QIcon(":/icons/configure-toolbars.png"), tr("Configure Toolbars..."));

	connect(actionShow, SIGNAL(toggled(bool)), this, SLOT(setVisible(bool)));
	connect(actionLock, SIGNAL(toggled(bool)), parent(), SLOT(actionLockToolBars(bool)));
	connect(actionConfigure, SIGNAL(triggered()), parent(), SLOT(actionToolbarsConfiguration()));

	menu.exec(event->globalPos());
}

void ToolBarWidget::reload()
{
	clear();

	QStringList actions = SettingManager::value("toolbars/" + objectName()).toStringList();

	for (int i = 0; i < actions.count(); ++i)
	{
		if (actions.at(i).isEmpty())
		{
			addSeparator();
		}
		else
		{
			addAction(ActionManager::getAction(actions.at(i)));
		}
	}
}

}
