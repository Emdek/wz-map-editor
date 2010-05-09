#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QtCore/QObject>
#include <QtGui/QAction>

namespace WZMapEditor
{

class ActionManager : public QObject
{
	Q_OBJECT

public:
	static void createInstance(QObject *parent = NULL);
	static void registerAction(QAction *action, QString name = QString());
	static void registerActions(QList<QAction*> actions);
	static void restoreDefaultShortcut(QAction *action);
	static void restoreDefaultShortcut(const QString &action);
	static QKeySequence getDefaultShortcut(QAction *action);
	static QKeySequence getDefaultShortcut(const QString &action);
	static QAction* getAction(const QString &action);
	static QList<QAction*> actions();

private:
	ActionManager(QObject *parent = NULL);

	void addAction(QAction *action, QString name = QString());
	QHash<QString, QAction*> getActions();

	QHash<QString, QAction*> m_actions;

	static ActionManager *m_instance;
};

}

#endif // ACTIONMANAGER_H
