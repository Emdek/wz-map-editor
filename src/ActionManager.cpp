#include "ActionManager.h"
#include "SettingManager.h"

namespace WZMapEditor
{

ActionManager* ActionManager::m_instance = NULL;

ActionManager::ActionManager(QObject *parent) : QObject(parent)
{
}

void ActionManager::createInstance(QObject *parent)
{
	m_instance = new ActionManager(parent);
}

void ActionManager::addAction(QAction *action, QString name)
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

void ActionManager::registerAction(QAction *action, QString name)
{
	m_instance->addAction(action, name);
}

void ActionManager::registerActions(QList<QAction*> actions)
{
	for (int i = 0; i < actions.count(); ++i)
	{
		m_instance->addAction(actions.at(i));
	}
}

void ActionManager::setShortcut(QAction *action, const QKeySequence &shortcut)
{
	QString key = m_instance->getActions().key(action);

	if (!key.isEmpty())
	{
		setShortcut(key, shortcut);
	}
}

void ActionManager::setShortcut(const QString &action, const QKeySequence &shortcut)
{
	if (m_instance->getActions().contains(action))
	{
		m_instance->getActions().value(action)->setShortcut(shortcut);

		SettingManager::setValue("actions/" + action, shortcut.toString());
	}
}

void ActionManager::restoreDefaultShortcut(QAction *action)
{
	QString key = m_instance->getActions().key(action);

	if (!key.isEmpty())
	{
		restoreDefaultShortcut(key);
	}
}

void ActionManager::restoreDefaultShortcut(const QString &action)
{
	if (m_instance->getActions().contains(action))
	{
		m_instance->getActions().value(action)->setShortcut(getDefaultShortcut(action));
	}
}

QKeySequence ActionManager::getDefaultShortcut(QAction *action)
{
	QString key = m_instance->getActions().key(action);

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
	return m_instance->getActions().value(action, NULL);
}

QHash<QString, QAction*> ActionManager::getActions()
{
	return m_actions;
}

QList<QAction*> ActionManager::actions()
{
	return m_instance->getActions().values();
}

}
