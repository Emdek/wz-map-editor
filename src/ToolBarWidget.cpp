#include "ToolBarWidget.h"

#include <QtCore/QSettings>

#include <QtGui/QMenu>

namespace WZMapEditor
{

ToolBarWidget::ToolBarWidget(QMainWindow *parent) : QToolBar(parent),
	m_dirty(true)
{
	connect(this, SIGNAL(configureRequested(ToolBarWidget*)), parent, SLOT(actionConfigureToolBars(ToolBarWidget*)));
	connect(this, SIGNAL(lockRequested(bool)), parent, SLOT(actionLockToolBars(bool)));
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
	connect(actionLock, SIGNAL(toggled(bool)), this, SIGNAL(lockRequested(bool)));
	connect(actionConfigure, SIGNAL(triggered()), this, SLOT(configure()));

	menu.exec(event->globalPos());
}

void ToolBarWidget::setObjectName(const QString &name)
{
	QObject::setObjectName(name);

	if (QSettings().contains(QString("ToolBars/%1").arg(name)))
	{
		m_actions = QSettings().value(QString("ToolBars/%1").arg(name), QStringList()).toStringList();
	}
	else
	{
		restoreDefaults();
	}

	m_dirty = true;
}

void ToolBarWidget::configure()
{
	emit configureRequested(this);
}

void ToolBarWidget::restoreDefaults()
{
	m_actions.clear();

	if (objectName() == "mainToolBar")
	{
		m_actions.append("actionNew");
		m_actions.append("actionOpen");
		m_actions.append(QString());
		m_actions.append("actionSave");
		m_actions.append("actionSaveAs");
		m_actions.append(QString());
		m_actions.append("actionUndo");
		m_actions.append("actionRedo");
	}
	else if (objectName() == "formatToolBar")
	{
		m_actions.append("actionBold");
		m_actions.append("actionItalic");
		m_actions.append("actionUnderline");
		m_actions.append(QString());
		m_actions.append("actionAlignLeft");
		m_actions.append("actionAlignCenter");
		m_actions.append("actionAlignRight");
		m_actions.append("actionAlignJustify");
		m_actions.append(QString());
		m_actions.append("actionOrderedList");
		m_actions.append("actionUnorderedList");
	}

	QSettings().remove(QString("ToolBars/%1").arg(objectName()));

	m_dirty = true;
}

void ToolBarWidget::load(QList<QAction*> actions)
{
	if (!m_dirty)
	{
		return;
	}

	QList<QAction*> currentActions = this->actions();

	for (int i = 0; i < currentActions.count(); ++i)
	{
		removeAction(currentActions.at(i));
	}

	for (int i = 0; i < m_actions.count(); ++i)
	{
		if (m_actions.at(i).isEmpty())
		{
			addSeparator();
		}
		else
		{
			for (int j = 0; j < actions.count(); ++j)
			{
				if (m_actions.at(i) == actions.at(j)->objectName())
				{
					addAction(actions.at(j));

					break;
				}
			}
		}
	}

	m_dirty = false;
}

void ToolBarWidget::setActions(const QStringList &actions)
{
	m_actions = actions;

	QSettings().setValue(QString("ToolBars/%1").arg(objectName()), m_actions);

	m_dirty = true;
}

}
