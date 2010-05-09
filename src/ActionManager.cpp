#include "ActionManager.h"

namespace WZMapEditor
{

ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
	m_defaultShortcuts["actionNew"] = QKeySequence::New;
	m_defaultShortcuts["actionOpen"] = QKeySequence::Open;
	m_defaultShortcuts["actionSave"] = QKeySequence::Save;
	m_defaultShortcuts["actionSaveAs"] = QKeySequence::SaveAs;
	m_defaultShortcuts["actionExit"] = QKeySequence::Quit;
	m_defaultShortcuts["actionUndo"] = QKeySequence::Undo;
	m_defaultShortcuts["actionRedo"] = QKeySequence::Redo;
	m_defaultShortcuts["actionHelp"] = QKeySequence::HelpContents;
	m_defaultShortcuts["actionTileset"] = QKeySequence(Qt::CTRL, Qt::Key_1);
	m_defaultShortcuts["actionTerrain"] = QKeySequence(Qt::CTRL, Qt::Key_2);
	m_defaultShortcuts["actionLand"] = QKeySequence(Qt::CTRL, Qt::Key_3);
	m_defaultShortcuts["actionTriangle"] = QKeySequence(Qt::CTRL, Qt::Key_4);
	m_defaultShortcuts["actionObjects"] = QKeySequence(Qt::CTRL, Qt::Key_5);
}

void ActionManager::registerAction(QAction *action, const QString &name)
{
	if (!action)
	{
		return;
	}

	if (name.isEmpty())
	{
		if (action->objectName().isEmpty())
		{
			QString tmpName = "action_%1";
			int number = 0;

			while (m_actions.contains(tmpName.arg(number)))
			{
				++number;
			}

			m_actions[tmpName.arg(number)] = action;
		}
		else
		{
			m_actions[action->objectName()] = action;
		}
	}
	else
	{
		m_actions[name] = action;
	}
}

void ActionManager::registerActions(QList<QAction*> actions)
{
	for (int i = 0; i < actions.count(); ++i)
	{
		registerAction(actions.at(i));
	}
}

void ActionManager::restoreDefaultShortcut(QAction *action)
{
	QString key = m_actions.key(action);

	if (!key.isEmpty())
	{
		return restoreDefaultShortcut(key);
	}
}

void ActionManager::restoreDefaultShortcut(const QString &action)
{
	if (m_defaultShortcuts.contains(action) && m_actions.contains(action))
	{
		m_actions[action]->setShortcut(m_defaultShortcuts[action]);
	}
}

QKeySequence ActionManager::getDefaultShortcut(QAction *action)
{
	QString key = m_actions.key(action);

	if (key.isEmpty())
	{
		return QKeySequence();
	}
	else
	{
		return getDefaultShortcut(key);
	}
}

QKeySequence ActionManager::getDefaultShortcut(const QString &action)
{
	return m_defaultShortcuts.value(action, QKeySequence());
}

QAction* ActionManager::getAction(const QString &action)
{
	return m_actions.value(action, NULL);
}


QList<QAction*> ActionManager::actions()
{
	return m_actions.values();
}

}
