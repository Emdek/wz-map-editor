#ifndef ACTIONSMANAGER_H
#define ACTIONSMANAGER_H

#include <QtCore/QObject>
#include <QtWidgets/QAction>

namespace WZMapEditor
{

class ActionsManager : public QObject
{
	Q_OBJECT

public:
	static void createInstance(QObject *parent = NULL);
	static void registerAction(QAction *action, QString name = QString());
	static void registerActions(QList<QAction*> getIdentifiers);
	static void setShortcut(QAction *action, const QKeySequence &shortcut);
	static void setShortcut(const QString &action, const QKeySequence &shortcut);
	static void restoreDefaultShortcut(QAction *action);
	static void restoreDefaultShortcut(const QString &action);
	static QAction* getAction(const QString &action);
	static QKeySequence getShortcut(QAction *action);
	static QKeySequence getShortcut(const QString &action);
	static QKeySequence getDefaultShortcut(QAction *action);
	static QKeySequence getDefaultShortcut(const QString &action);
	static QStringList getIdentifiers();
	static bool hasShortcut(const QKeySequence &shortcut, const QString &excludeAction = QString());

private:
	explicit ActionsManager(QObject *parent = NULL);

	void addAction(QAction *action, QString name = QString());

	static ActionsManager *m_instance;
	static QHash<QString, QAction*> m_actions;
};

}

#endif
