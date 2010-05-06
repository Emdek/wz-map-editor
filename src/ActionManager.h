#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QtCore/QObject>

namespace WZMapEditor
{

class ActionManager : public QObject
{
	Q_OBJECT

public:
	ActionManager(QObject *parent = 0);

signals:

public slots:

};

}

#endif // ACTIONMANAGER_H
