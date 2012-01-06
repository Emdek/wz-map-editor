#ifndef MAPPARSER_H
#define MAPPARSER_H

#include "Map.h"

#include <QtCore/QObject>


namespace WZMapEditor
{

class MapParser : public QObject
{
	Q_OBJECT

public:
	MapParser(const QString &filePath, QObject *parent = NULL);

	Map* map();

protected:
	int deserializeMap(QDataStream &stream);
	int deserializeGame(QDataStream &stream);
	int deserializeTerrain(QDataStream &stream);
	int deserializeStructures(QDataStream &stream);
	int deserializeDroids(QDataStream &stream);
	int deserializeFeatures(QDataStream &stream);

private:
	Map *m_map;
	QVector<QVector<MapTile> > m_tiles;
};

}

#endif // MAPPARSER_H
