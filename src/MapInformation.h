#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H

#include "Tileset.h"

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QSize>


namespace WZMapEditor
{

enum MapObjectType
{
	MapObjectTypeFeature,
	MapObjectTypeStructure,
	MapObjectTypeDroid,
	MapObjectTypeObject
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

	void setSize(const QSize &size);
	void setTilesetType(TilesetType tilesetType);
	QSize size();
	TilesetType tilesetType();

private:
	QString m_name;
	QSize m_size;
	TilesetType m_tilesetType;
	QList<QList<MapTile> > m_tiles;
	QList<QList<Gateway> > m_gateways;
};

}

#endif // MAPINFORMATION_H
