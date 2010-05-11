#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H

#include <QtCore/QObject>
#include <QtCore/QPoint>


namespace WZMapEditor
{

enum MapObjectType
{
	MapObjectTypeFeature,
	MapObjectTypeStructure,
	MapObjectTypeDroid,
	MapObjectTypeObject
};


enum TilesetType
{
	TilesetTypeArizona = 0,
	TilesetTypeUrban = 1,
	TilesetTypeRockies = 2
};

struct MapTile
{

};

struct Gateway
{
	QPoint start;
	QPoint end;
};

class MapInformation : public QObject
{
	Q_OBJECT

public:
	MapInformation(QObject *parent = NULL);

private:
	QString m_name;
	TilesetType m_tileset;
	QList<QList<MapTile> > m_tiles;
	QList<QList<Gateway> > m_gateways;
};

}

#endif // MAPINFORMATION_H
