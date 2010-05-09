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
	ActionManager(QObject *parent = NULL);

	void registerAction(QAction *action, QString name = QString());
	void registerActions(QList<QAction*> actions);
	void restoreDefaultShortcut(QAction *action);
	void restoreDefaultShortcut(const QString &action);
	QKeySequence getDefaultShortcut(QAction *action);
	QKeySequence getDefaultShortcut(const QString &action);
	QAction* getAction(const QString &action);
	QList<QAction*> actions();

private:
	QHash<QString, QAction*> m_actions;

};

}

#endif // ACTIONMANAGER_H
