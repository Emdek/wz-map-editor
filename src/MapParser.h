#ifndef MAPPARSER_H
#define MAPPARSER_H

#include "MapInformation.h"

#include <QtCore/QObject>


namespace WZMapEditor
{

class MapParser : public QObject
{
	Q_OBJECT

public:
	MapParser(const QString &filePath, QObject *parent = NULL);

	MapInformation* map();

protected:
	int deserializeMap(QDataStream &stream);
	int deserializeGame(QDataStream &stream);
	int deserializeTerrain(QDataStream &stream);
	int deserializeStructures(QDataStream &stream);
	int deserializeDroids(QDataStream &stream);
	int deserializeFeatures(QDataStream &stream);

private:
	MapInformation *m_mapInformation;
	QVector<QVector<MapTile> > m_tiles;
};

}

#endif // MAPPARSER_H
