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
	ActionManager(QObject *parent = 0);

private:
	QHash<QString, QAction*> m_actions;

};

}

#endif // ACTIONMANAGER_H
