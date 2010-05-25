#include "MapParser.h"

#include <QtCore/QTime>

#include <QtEndian>


#include <QtDebug>


namespace WZMapEditor
{

static inline bool checkHeaderType(QDataStream& stream, const char* type, int cmpLength)
{
	char fileType[4] = {0}; // Reading the first 4 bytes
	int i;

	if (stream.readRawData(fileType, 4) != 4)
	{
		return -1;
	}

	for (i = 0; i < cmpLength; ++i)
	{
		if (fileType[i] != type[i])
		{
			break;
		}
	}

	return !(i < cmpLength);
}

/* Function to reduce code duplication in MapInformation::deserialize
 * Not meant to be used elsewhere
 */
static inline int openNSet(const QString &fileName, QFile &file, QDataStream &dataStream)
{
	dataStream.setDevice(NULL); // Might not be needed.
	file.close();
	file.setFileName(fileName);

	if (!file.exists())
	{
		qCritical() << QString("MapInformation::deserialize - Could not find \"%1\" in directory.").arg(fileName);

		return -1;
	}

	file.open(QIODevice::ReadOnly);

	if (!file.isOpen())
	{
		qCritical() << QString("MapInformation::deserialize - Could not open \"%1\".").arg(fileName);

		return -1;
	}

	dataStream.setDevice(&file);

	if (dataStream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapInformation::deserialize - problem setting stream.");

		return -1;
	}

	return 0;
}

MapParser::MapParser(const QString &filePath, QObject *parent) : QObject(parent),
	m_mapInformation(new MapInformation(parent))
{
	QTime test;
	QFile file;
	QFileInfo fileInfo(filePath);
	QDataStream dataStream;
	QDir filesDir = fileInfo.absoluteDir();
	int retVal;

	test.start();

	if (!filesDir.cd(fileInfo.baseName()))
	{
		qCritical() << "Couldn't find map directory.";

		return;
	}

	if (openNSet(filesDir.filePath("game.map"), file, dataStream) != 0)
	{
		qCritical() << "Couldn't open map.";

		return;
	}

	retVal = deserializeMap(dataStream);

	if (retVal != 0)
	{
		return;
	}

	if (openNSet(filesDir.filePath("ttypes.ttp"), file, dataStream) != 0)
	{
		qCritical() << "Couldn't open ttypes.ttp.";
	}
	else
	{
		deserializeTerrain(dataStream);
	}

	if (openNSet(fileInfo.filePath(), file, dataStream) != 0)
	{
		qCritical() << "Couldn't open .gam file.";
	}

	deserializeGame(dataStream);

	if (openNSet(filesDir.filePath("struct.bjo"),file,dataStream) == 0)
	{
		deserializeStructures(dataStream);
	}

	if (openNSet(filesDir.filePath("dinit.bjo"), file, dataStream) == 0)
	{
		deserializeDroids(dataStream);
	}

	if (openNSet(filesDir.filePath("feat.bjo"), file, dataStream) == 0)
	{
		deserializeFeatures(dataStream);
	}

	file.close();

	qDebug() << "Deserialize time: " << test.elapsed();

	if (!retVal)
	{
		m_mapInformation->setModified(false);
		m_mapInformation->setTiles(m_tiles);
	}
}

int MapParser::deserializeMap(QDataStream &stream)
{
	QSize size;
	quint32 u32;
	quint16 texture;
	quint8 height, x1, x2, y1, y2;
	QVector<MapTile> mapRow;
	QList<Gateway> gateways;
	MapTile tile;
	int i, j;

	stream.setByteOrder(QDataStream::LittleEndian);

	if (!checkHeaderType(stream, "map", 3))
	{
		qCritical() << "MapParser::deserializeMap - Invalid header.";

		return -1;
	}

	stream >> u32;

	if (u32 <= 9/*MAP_VERSION_9*/ || stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeMap - Unsupported map version %1.").arg(u32);

		return -1;
	}

	stream >> size.rwidth() >>  size.rheight();

	if (!size.isValid() || stream.status() != QDataStream::Ok || (size.width() * size.height()) > MAP_MAXAREA)
	{
		qCritical() << QString("MapParser::deserializeMap - Invalid map size %1 x %1.").arg(size.width()).arg(size.height());

		return -1;
	}

	m_mapInformation->setSize(size);

	m_tiles.reserve(size.width());
	mapRow.reserve(m_mapInformation->size().height());

	for (j = 0; j < m_mapInformation->size().height(); ++j)
	{
		mapRow.clear();

		for (i = 0; i < m_mapInformation->size().width(); ++i)
		{
			stream >> texture >> height;
			tile.texture = texture;
			tile.height = height;
			tile.position.rx() = i;
			tile.position.ry() = j;
			mapRow.push_back(tile);
		}

		m_tiles.push_back(mapRow);
	}

	if (stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeMap - Error reading tiles.");

		return -1;
	}

	stream >> u32;	// Read gateway version

	if (u32 != 1 /* version != 1*/ || stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeMap - Error reading gateway version.");

		return -1;
	}

	stream >> u32;	// Read number of gateways

	if (stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeMap - Error reading number of gateways.");

		return -1;
	}

	for (; u32 > 0; --u32) // for each gateway
	{
		stream >> x1 >> y1 >> x2 >> y2;

		gateways.append(Gateway(x1, y1, x2, y2));
	}

	m_mapInformation->setGateways(gateways);

	if (stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeMap - Error reading gateways.");

		return -1;
	}

	return 0;
}

int MapParser::deserializeGame(QDataStream &stream)
{
	char levelName[21] = {0};
	quint32 u32, gameTime, gameType, terrainVersion, top, left, bottom, right;
	QRect scrollLimits;

	if (!checkHeaderType(stream,"gam",3))
	{
		qCritical() << "MapParser::deserializeGame - Invalid header.";

		return -1;
	}

	// Read stream map version
	stream >> u32;

	/*
	 * Map version >=35 use big endian
	 * Checking that version <=34 is what is generaly used
	 */
	if ((u32&~0xFF) != 0)
	{
		if (stream.byteOrder() == QDataStream::LittleEndian)
		{
			stream.setByteOrder(QDataStream::BigEndian);
			qToBigEndian(u32, (uchar*)&terrainVersion); // FIXME? nasty cast
		}
		else
		{
			stream.setByteOrder(QDataStream::LittleEndian);
			qToLittleEndian(u32, (uchar*)&terrainVersion); // FIXME? nasty cast
		}
	}
	else
	{
		terrainVersion = u32;
	}

	stream >> gameTime >> gameType >> left >> bottom >> right >> top;

	if (stream.status() != QDataStream::Ok)
	{

	}

	scrollLimits.setLeft(left);
	scrollLimits.setRight(right);
	scrollLimits.setTop(top);
	scrollLimits.setBottom(bottom);

	stream.readRawData(levelName, 20);

	levelName[20] = '\0';

	m_mapInformation->setScrollLimits(scrollLimits);
	m_mapInformation->setName(QString(levelName));
	m_mapInformation->setTime(gameTime);
	m_mapInformation->setType(gameType);

	return 0;
}

int MapParser::deserializeTerrain(QDataStream &stream)
{
	unsigned int i;
	quint32 numTerrainTypes, terrainVersion;
	quint8 terrainType;
	QList<quint8> terrainTypes;

	stream.setByteOrder(QDataStream::LittleEndian);

	if (!checkHeaderType(stream,"ttyp",4))
	{
		qCritical() << "MapParser::deserializeTerrain - Invalid header.";

		return -1;
	}

	stream >> terrainVersion >> numTerrainTypes;

	if (stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeTerrain - Error reading terrain fields.");

		return -1;
	}

	for (i = 0; i < numTerrainTypes; ++i)
	{
		stream >> terrainType;

		terrainTypes.append(terrainType);
	}

	if (stream.status() != QDataStream::Ok)
	{
		qCritical() << QString("MapParser::deserializeTerrain - Error reading terrain types.");

		return -1;
	}

	m_mapInformation->setTerrainTypes(terrainTypes);

	return 0;
}

int MapParser::deserializeStructures(QDataStream &stream)
{
	QList<Entity> structures;
	quint32 u32,i;
	quint32 structVersion;
	quint64 pos; // debug var
	char charArray[60];
	Entity entity;

	if (!checkHeaderType(stream, "stru", 4))
	{

		qWarning() << "MapParser::deserializeStructures - Invalid header.";

		return -1;
	}

	stream >> structVersion >> i;

	for (; i > 0; --i)
	{
		pos = stream.device()->pos();

		if (structVersion <= 19)
		{
			if (stream.readRawData(charArray, 40) != 40)
			{
				qWarning() << QString("MapParser::deserializeStructures - Error reading name of structure %1/%2").arg(structures.size()).arg(structures.size() + i - 1);

				return -1;
			}
		}
		else
		{
			if (stream.readRawData(charArray, 60) != 60)
			{
				qWarning() << QString("MapParser::deserializeStructures - Error reading name of structure %1/%2").arg(structures.size()).arg(structures.size() + i - 1);

				return -1;
			}
		}

		entity.name = QString(charArray);
		stream >> entity.id;
		stream >> u32;
		entity.position.setX(u32);
		stream >> u32;
		entity.position.setY(u32);
		stream >> u32;
		entity.position.setZ(u32);
		stream >> entity.direction >> entity.player;

		if (stream.status() != QDataStream::Ok)
		{
			qWarning() << QString("MapParser::deserializeStructures - Error reading fields of structure %1/%2").arg(structures.size()).arg(structures.size() + i - 1);

			return -1;
		}

		stream.skipRawData(14 * 4);	// Returns bytes skipped or -1 on error
		/* Skip the following fields:
		 * U32 : BOOL inFire
		 * U32 : burnStart
		 * U32 : burnDamage
		 * U8 : status - causes structure padding
		 * U8 : structure padding
		 * U8 : structure padding
		 * U8 : structure padding
		 * S32 : currentBuildPts - aligned on 4 byte boundary
		 * U32 : body
		 * U32 : armour
		 * U32 : resistance
		 * U32 : dummy1
		 * U32 : subjectInc
		 * U32 : timeStarted
		 * U32 : output
		 * U32 : capacity
		 * U32 : quantity
		 */

		if (structVersion >= 12)
		{
			stream.skipRawData(7 * 4);
			/* U32 : factoryInc
			 * U8 : loopsPerformed - causes structure padding
			 * U8 : structure padding
			 * U8 : structure padding
			 * U8 : structure padding
			 * U32 : powerAccrued - aligned on 4 byte boundary
			 * U32 : dummy2
			 * U32 : droidTimeStarted
			 * U32 : timeToBuild
			 * U32 : timeStartHold
			 */
		}

		if (structVersion >= 14) // Skip visibility
		{
			stream.skipRawData(8 * 1);
		}

		if (structVersion >= 15)
		{
			stream.skipRawData(60 * 1);
			// If version < 20, then this causes no padding, but the short below
			// will still cause two bytes padding; however, if version >= 20, we
			// will cause 4 bytes padding, but the short below will eat 2 of them,
			// leaving us again with only two bytes padding before the next word.
		}

		if (structVersion >= 17)
		{
			stream.skipRawData(1 * 2);
		}

		if (structVersion >= 15) // Structure padding
		{
			stream.skipRawData(1 * 2);
		}

		if (structVersion >= 21)
		{
			stream.skipRawData(1 * 4);
		}

		entity.type = MapObjectTypeStructure;

		if (entity.player > MAX_PLAYERS)
		{
			qWarning() << QString("MapParser::deserializeStructures - Invalid player field of structure %1/%2").arg(structures.size()).arg(structures.size() + i - 1);

			return -1;
		}

		if (entity.position.x() >= (m_tiles.size() * TILE_WIDTH) || entity.position.y() >= (m_tiles.at(0).size() * TILE_HEIGHT))
		{
			qWarning() << QString("MapParser::deserializeStructures - Invalid position fields of structure %1/%2").arg(structures.size()).arg(structures.size()+i-1);

			return -1;
		}

		if (stream.status() != QDataStream::Ok)
		{
			qWarning() << QString("MapParser::deserializeStructures - Corrupted or invalid structure file detected (structure %1/%2)").arg(structures.size()).arg(structures.size()+i-1);

			return -1;
		}

		structures.push_back(entity);
	}

	m_mapInformation->setStructures(structures);

	return 0;
}

int MapParser::deserializeDroids(QDataStream &stream)
{
	QList<Entity> droids;
	quint32 u32, i;
	quint32 droidVersion;
	char charArray[60];
	Entity entity;
	QVector<bool> playerPresent;
	playerPresent.resize(MAX_PLAYERS);

	if (!checkHeaderType(stream, "dint", 4))
	{
		qWarning() << "MapParser::deserializeDroids - Invalid header.";

		return -1;
	}

	stream >> droidVersion >> i;

	for (; i > 0; --i)
	{
		if (droidVersion <= 19)
		{
			if (stream.readRawData(charArray, 40) != 40)
			{
				qWarning() << QString("MapParser::deserializeDroids - Error reading name of droid %1/%2").arg(droids.size()).arg(droids.size() + i - 1);

				return -1;
			}
		}
		else
		{
			if (stream.readRawData(charArray, 60) != 60)
			{
				qWarning() << QString("MapParser::deserializeDroids - Error reading name of droid %1/%2").arg(droids.size()).arg(droids.size()+i-1);

				return -1;
			}
		}

		entity.name = QString(charArray);
		stream >> entity.id;
		stream >> u32;
		entity.position.setX(u32);
		stream >> u32;
		entity.position.setY(u32);
		stream >> u32;
		entity.position.setZ(u32);
		stream >> entity.direction >> entity.player;

		if (stream.status() != QDataStream::Ok)
		{
			qWarning() << QString("MapParser::deserializeDroids - Error reading fields of droid %1/%2").arg(droids.size()).arg(droids.size()+i-1);

			return -1;
		}

		stream.skipRawData(3 * 4);

		entity.type = MapObjectTypeDroid;

		// sanity check
		if (entity.position.x() >= (m_tiles.size() * TILE_WIDTH) || entity.position.y() >= (m_tiles.at(0).size() * TILE_HEIGHT))
		{
			qWarning() << QString("MapParser::deserializeDroids - Invalid position fields of droid %1/%2").arg(droids.size()).arg(droids.size() + i - 1);

			return -1;
		}

		droids.push_back(entity);
	}

	foreach (entity, droids)
	{
		if (entity.name.compare("ConstructorDroid") == 0 || entity.name.compare("ConstructionDroid") == 0)
		{
			playerPresent[entity.player] = true;
		}
	}

	m_mapInformation->setDroids(droids);
	m_mapInformation->setPlayerPresent(playerPresent);

	return 0;
}

int MapParser::deserializeFeatures(QDataStream &stream)
{
	QList<Entity> features;
	quint32 featVersion, i;
	quint32 u32;
	Entity entity;
	char charArray[60];

	if (!checkHeaderType(stream, "feat", 4))
	{
		qWarning() << "MapParser::deserializeFeats - Invalid header.";

		return -1;
	}

	stream >> featVersion >> i;

	for(; i > 0; --i)
	{
		if (featVersion <= 19)
		{
			if (stream.readRawData(charArray, 40) != 40)
			{
				qWarning() << QString("MapParser::deserializeFeats - Error reading name of feat %1/%2").arg(features.size()).arg(features.size() + i - 1);

				return -1;
			}
		}
		else
		{
			if (stream.readRawData(charArray, 60) != 60)
			{
				qWarning() << QString("MapParser::deserializeFeats - Error reading name of feat %1/%2").arg(features.size()).arg(features.size() + i - 1);

				return -1;
			}
		}

		entity.name = QString(charArray);
		stream >> entity.id;
		stream >> u32;
		entity.position.setX(u32);
		stream >> u32;
		entity.position.setY(u32);
		stream >> u32;
		entity.position.setZ(u32);
		stream >> entity.direction >> entity.player;

		if (stream.status() != QDataStream::Ok)
		{
			qWarning() << QString("MapParser::deserializeFeats - Error reading fields of feat %1/%2").arg(features.size()).arg(features.size() + i - 1);

			return -1;
		}

		stream.skipRawData(3 * 4);

		if (featVersion >= 14) // Skip visibility
		{
			stream.skipRawData(8 * 1);
		}

		if (entity.position.x() >= (m_tiles.size() * TILE_WIDTH) || entity.position.y() >= (m_tiles.at(0).size() * TILE_HEIGHT))
		{
			qWarning() << QString("MapParser::deserializeFeats - Invalid position fields of feat %1/%2").arg(features.size()).arg(features.size() + i - 1);

			return -1;
		}

		if (stream.status() != QDataStream::Ok)
		{
			qWarning() << QString("MapParser::deserializeFeats - Corrupted or invalid feature file detected (feat %1/%2)").arg(features.size()).arg(features.size() + i - 1);

			return -1;
		}

		features.push_back(entity);
	}

	m_mapInformation->setFeatures(features);

	return 0;
}

MapInformation* MapParser::map()
{
	return m_mapInformation;
}

}
