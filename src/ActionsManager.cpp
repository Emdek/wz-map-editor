#include "ActionsManager.h"
#include "SettingsManager.h"

namespace WZMapEditor
{

ActionsManager* ActionsManager::m_instance = NULL;
QHash<QString, QAction*> ActionsManager::m_actions;

ActionsManager::ActionsManager(QObject *parent) : QObject(parent)
{
}

void ActionsManager::createInstance(QObject *parent)
{
	m_instance = new ActionsManager(parent);
}

void ActionsManager::addAction(QAction *action, QString name)
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

	action->setObjectName(name);
	action->setShortcut(QKeySequence(SettingsManager::getDefaultValue("Actions/" + name).toString()));

	m_actions[name] = action;
}

void ActionsManager::registerAction(QAction *action, QString name)
{
	m_instance->addAction(action, name);
}

void ActionsManager::registerActions(QList<QAction*> actions)
{
	for (int i = 0; i < actions.count(); ++i)
	{
		m_instance->addAction(actions.at(i));
	}
}

void ActionsManager::setShortcut(QAction *action, const QKeySequence &shortcut)
{
	QString key = m_actions.key(action);

	if (!key.isEmpty())
	{
		setShortcut(key, shortcut);
	}
}

void ActionsManager::setShortcut(const QString &action, const QKeySequence &shortcut)
{
	if (m_actions.contains(action))
	{
		m_actions.value(action)->setShortcut(shortcut);

		SettingsManager::setValue("Actions/" + action, shortcut.toString());
	}
}

void ActionsManager::restoreDefaultShortcut(QAction *action)
{
	QString key = m_actions.key(action);

	if (!key.isEmpty())
	{
		restoreDefaultShortcut(key);
	}
}

void ActionsManager::restoreDefaultShortcut(const QString &action)
{
	if (m_actions.contains(action))
	{
		m_actions.value(action)->setShortcut(getDefaultShortcut(action));
	}
}

QAction* ActionsManager::getAction(const QString &action)
{
	return m_actions.value(action, NULL);
}

QKeySequence ActionsManager::getShortcut(QAction *action)
{
	const QString key = m_actions.key(action);

	if (key.isEmpty())
	{
		return QKeySequence();
	}
	else
	{
		return m_actions[key]->shortcut();
	}
}

QKeySequence ActionsManager::getShortcut(const QString &action)
{
	return QKeySequence(SettingsManager::getValue("Actions/" + action).toString());
}

QKeySequence ActionsManager::getDefaultShortcut(QAction *action)
{
	const QString key = m_actions.key(action);

	if (key.isEmpty())
	{
		return QKeySequence();
	}
	else
	{
		return getDefaultShortcut(key);
	}
}

QKeySequence ActionsManager::getDefaultShortcut(const QString &action)
{
	return QKeySequence(SettingsManager::getDefaultValue("Actions/" + action).toString());
}

QStringList ActionsManager::getActions()
{
	return m_actions.keys();
}

bool ActionsManager::hasShortcut(const QKeySequence &shortcut, const QString &excludeAction)
{
	if (shortcut.isEmpty())
	{
		return false;
	}

	const QList<QAction*> actions = m_actions.values();

	for (int i = 0; i < actions.count(); ++i)
	{
		if (actions.at(i) && actions.at(i)->shortcut() == shortcut && (excludeAction.isEmpty() || actions.at(i)->objectName() != excludeAction))
		{
			return true;
		}
	}

	return false;
}

}
