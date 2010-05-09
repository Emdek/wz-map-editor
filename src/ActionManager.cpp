#include "ActionManager.h"
#include "SettingManager.h"


namespace WZMapEditor
{

ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
}

void ActionManager::registerAction(QAction *action, QString name)
{
	if (!action)
	{
		return;
	}

	if (name.isEmpty())
	{
		if (action->objectName().isEmpty())
		{
			QString tmpName = "_%1";
			int number = 0;

			while (m_actions.contains(tmpName.arg(number)))
			{
				++number;
			}

			name = tmpName.arg(number);
		}
		else
		{
			name = action->objectName().mid(6);
		}
	}

	action->setShortcut(QKeySequence(SettingManager::defaultValue("actions/" + name).toString()));

	m_actions[name] = action;
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
	if (m_actions.contains(action))
	{
		m_actions[action]->setShortcut(getDefaultShortcut(action));
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
	return QKeySequence(SettingManager::defaultValue("actions/" + action).toString());
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
