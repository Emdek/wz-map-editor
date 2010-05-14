#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H

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

class Tileset;

class MapInformation : public QObject
{
	Q_OBJECT

public:
	MapInformation(QObject *parent = NULL);

	void setSize(const QSize &size);
	QSize size();

private:
	QString m_name;
	QSize m_size;
	Tileset *m_tileset;
	QList<QList<MapTile> > m_tiles;
	QList<QList<Gateway> > m_gateways;
};

}

#endif // MAPINFORMATION_H
