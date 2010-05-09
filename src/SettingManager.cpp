#include "SettingManager.h"

#include <QtCore/QSettings>
#include <QtGui/QKeySequence>

namespace WZMapEditor
{

SettingManager* SettingManager::m_instance = NULL;

SettingManager::SettingManager(QObject *parent) : QObject(parent)
{
	m_defaultSettings["actions/New"] = QVariant(QKeySequence(QKeySequence::New).toString());
	m_defaultSettings["actions/Open"] = QVariant(QKeySequence(QKeySequence::Open).toString());
	m_defaultSettings["actions/Save"] = QVariant(QKeySequence(QKeySequence::Save).toString());
	m_defaultSettings["actions/SaveAs"] = QVariant(QKeySequence(QKeySequence::SaveAs).toString());
	m_defaultSettings["actions/Exit"] = QVariant(QKeySequence(QKeySequence::Quit).toString());
	m_defaultSettings["actions/Undo"] = QVariant(QKeySequence(QKeySequence::Undo).toString());
	m_defaultSettings["actions/Redo"] = QVariant(QKeySequence(QKeySequence::Redo).toString());
	m_defaultSettings["actions/Help"] = QVariant(QKeySequence(QKeySequence::HelpContents).toString());
	m_defaultSettings["actions/Tileset"] = QVariant(QKeySequence(Qt::CTRL, Qt::Key_1).toString());
	m_defaultSettings["actions/Terrain"] = QVariant(QKeySequence(Qt::CTRL, Qt::Key_2).toString());
	m_defaultSettings["actions/Land"] = QVariant(QKeySequence(Qt::CTRL, Qt::Key_3).toString());
	m_defaultSettings["actions/Triangle"] = QVariant(QKeySequence(Qt::CTRL, Qt::Key_4).toString());
	m_defaultSettings["actions/Objects"] = QVariant(QKeySequence(Qt::CTRL, Qt::Key_5).toString());

///FIXME insert settings here...
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
