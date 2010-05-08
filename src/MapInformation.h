#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H

#include <QtCore/QObject>

namespace WZMapEditor
{

struct MapTile
{

};

class MapInformation : public QObject
{
	Q_OBJECT

public:
	MapInformation(QObject *parent = NULL);

private:
	QList<QList<MapTile> > m_tiles;
};

}

#endif // MAPINFORMATION_H
