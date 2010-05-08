#include "SettingManager.h"

#include <QtCore/QStringList>

namespace WZMapEditor
{

SettingManager* SettingManager::m_instance = NULL;

SettingManager::SettingManager(QObject *parent) : QObject(parent)
{
/// insert settings here...
}

void SettingManager::createInstance(QObject *parent)
{
	m_instance = new SettingManager(parent);
}

void SettingManager::restore(const QString &key)
{
	QSettings().setValue(key, m_instance->defaultValue(key));
}

QVariant SettingManager::keyValue(const QString &key)
{
	return m_defaultSettings[key];
}

QVariant SettingManager::defaultValue(const QString &key)
{
	return m_instance->keyValue(key);
}

QVariant SettingManager::value(const QString &key)
{
	return QSettings().value(key, defaultValue(key));
}

QStringList SettingManager::valueKeys()
{
	return m_defaultSettings.keys();
}

QStringList SettingManager::keys()
{
	return m_instance->valueKeys();
}

}
