#ifndef SETTINGMANAGER_H
#define SETTINGMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

namespace WZMapEditor
{

class SettingManager : public QObject
{
	Q_OBJECT

public:
	static void createInstance(QObject *parent = NULL);
	static void restore(const QString &key);
	static void setValue(const QString &key, const QVariant &value);
	static QVariant defaultValue(const QString &key);
	static QVariant value(const QString &key);
	static QStringList keys();

private:
	SettingManager(QObject *parent = NULL);

	QVariant keyValue(const QString &key);
	QStringList valueKeys();

	QHash<QString, QVariant> m_defaultSettings;

	static SettingManager *m_instance;
};

}

#endif // SETTINGMANAGER_H
