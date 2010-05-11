#ifndef TILESET_H
#define TILESET_H

#include <QtCore/QObject>


namespace WZMapEditor
{

class Tileset : public QObject
{
	Q_OBJECT

public:
	Tileset(QObject *parent = 0);

};

}

#endif // TILESET_H
