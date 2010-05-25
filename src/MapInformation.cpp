#include "MapInformation.h"
#include <QtDebug>
#include <QtEndian>

namespace WZMapEditor
{

MapTile::MapTile()
	: gateway(false),rotation(0),texture(0),
	height(0),flip(FlipTypeNone)
{
}
Gateway::Gateway(int x0, int y0, int x1, int y1)
{
	start = QPoint(x0,y0);
	end = QPoint(x1, y1);
}

MapInformation::MapInformation(QObject *parent) : QObject(parent),
	m_size(10, 10),
	m_tilesetType(TilesetTypeArizona),
	m_levelName("Unnamed"),
	m_players(0)
{
}

/* Function to reduce code duplication in MapInformation::deserialize
 * Not meant to be used elsewhere
 */
static inline int openNSet(QString fileName,QFile& file, QDataStream& ds )
{
	ds.setDevice(NULL); // Might not be needed.
	file.close();
	file.setFileName(fileName);
	if(!file.exists())
	{
		qCritical() << QString("MapInformation::deserialize - Could not find \"%1\" in directory.").arg(fileName);
		return -1;
	}
	file.open(QIODevice::ReadOnly);
	if(!file.isOpen())
	{
		qCritical() << QString("MapInformation::deserialize - Could not open \"%1\".").arg(fileName);
		return -1;
	}
	ds.setDevice(&file);
	if(ds.status()!=QDataStream::Ok)
	{
		qCritical() << QString("MapInformation::deserialize - problem setting stream.");
		return -1;
	}
	return 0;
}

int MapInformation::deserialize(const QFileInfo& fileNfo)
{
	QFile file;
	QDataStream ds;
	QDir filesDir=fileNfo.absoluteDir();
	int retVal;

	if(!filesDir.cd(fileNfo.baseName()))
	{
		qCritical() << "Couldn't find map directory.";
		return -1;
	}

	mapClear();

	if(openNSet(filesDir.filePath("game.map"),file,ds)!=0)
	{
		qCritical() << "Couldn't open map.";
		return -1;
	}
	retVal = deserializeMap(ds);


	if(retVal != 0)
		return retVal;

	if(openNSet(filesDir.filePath("ttypes.ttp"),file,ds)!=0)
	{
		qCritical() << "Couldn't open ttypes.ttp.";
	}
	else
	{
		deserializeTerrain(ds);
	}

	if(openNSet(fileNfo.filePath(),file,ds)!=0)
	{
		qCritical() << "Couldn't open .gam file.";
	}
	deserializeGame(ds);

	if(openNSet(filesDir.filePath("struct.bjo"),file,ds)==0)
	{
		deserializeStructures(ds);
	}
	if(openNSet(filesDir.filePath("dinit.bjo"),file,ds)==0)
	{
		deserializeDroids(ds);
	}
	if(openNSet(filesDir.filePath("feat.bjo"),file,ds)==0)
	{
		deserializeFeats(ds);
	}
	file.close();
	return retVal;
}
static inline bool checkHeaderType(QDataStream& in,const char* type, int cmpLength)
{
	char fileType[4]={0}; // Reading the first 4 bytes
	int i;


	if(in.readRawData(fileType,4)!=4)
	{
		return -1;
	}
	for(i=0;i<cmpLength;i++)
	{
		if(fileType[i]!=type[i])
		{
			break;
		}
	}
	return !i<cmpLength;
}

int MapInformation::deserializeMap(QDataStream& in)
{
	quint32 u32;
	quint16 texture;
	quint8 height,x1,x2,y1,y2;
	QVector<MapTile> mapRow;
	MapTile tile;
	int i,j;

	in.setByteOrder(QDataStream::LittleEndian);

	if(!checkHeaderType(in,"map",3))
	{
		mapClear();
		qCritical() << "MapInformation::deserializeMap - Invalid header.";
		return -1;
	}

	in >> u32;
	if(u32 <= 9/*MAP_VERSION_9*/ || in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Unsupported map version %1.").arg(u32);
		return -1;
	}

	in >> m_size.rwidth() >>  m_size.rheight();
	if( !m_size.isValid() || in.status()!=QDataStream::Ok
		|| m_size.width() * m_size.height() > 256*256 /*MAP_MAXAREA*/)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Invalid map size %1 x %1.").arg(m_size.width()).arg(m_size.height());
		return -1;
	}

	m_tiles.reserve(m_size.width());
	mapRow.reserve(m_size.height());
	for( j = 0; j < m_size.height(); j++)
	{
		mapRow.clear();
		for(i = 0; i < m_size.width(); i++)
		{
			in >> texture >> height;
			tile.texture = texture;
			tile.height = height;
			tile.position.rx() = i;
			tile.position.ry() = j;
			mapRow.push_back(tile);
		}
		m_tiles.push_back(mapRow);
	}
	if(in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Error reading tiles.");
		return -1;
	}

	in >> u32;	// Read gateway version
	if(u32 != 1 /* version != 1*/ || in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Error reading gateway version.");
		return -1;
	}

	in >> u32;	// Read number of gateways
	if(in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Error reading number of gateways.");
		return -1;
	}
	for(;u32>0;u32--) // for each gateway
	{
		in >> x1 >> y1 >> x2 >> y2;
		m_gateways.append(Gateway( x1, y1, x2, y2));
	}
	if(in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeMap - Error reading gateways.");
		return -1;
	}
	return 0;
}

int MapInformation::deserializeGame(QDataStream& in)
{
	char levelName[21]={0};
	quint32 u32, top, left, bottom, right;

	if(!checkHeaderType(in,"gam",3))
	{
		mapClear();
		qCritical() << "MapInformation::deserializeGame - Invalid header.";
		return -1;
	}

	// Read in map version
	in >> u32;

	/*
	 * Map version >=35 use big endian
	 * Checking that version <=34 is what is generaly used
	 */
	if((u32&~0xFF)!=0)
	{
		if(in.byteOrder()==QDataStream::LittleEndian)
		{
			in.setByteOrder(QDataStream::BigEndian);
			qToBigEndian(u32,(uchar*)&terrainVersion); // FIXME? nasty cast
		}
		else
		{
			in.setByteOrder(QDataStream::LittleEndian);
			qToLittleEndian(u32,(uchar*)&terrainVersion); // FIXME? nasty cast
		}
	}
	else
	{
		terrainVersion = u32;
	}

	in >> m_gameTime >> m_gameType;

	in >> left >> bottom >> right >> top;
	if(in.status()!=QDataStream::Ok)
	{

	}
	m_scrollLimits.setLeft(left);
	m_scrollLimits.setRight(right);
	m_scrollLimits.setTop(top);
	m_scrollLimits.setBottom(bottom);

	in.readRawData(levelName,20);
	levelName[20]='\0';

	m_levelName = QString(levelName);

	return 0;
}

int MapInformation::deserializeTerrain(QDataStream& in)
{
	unsigned int i;

	in.setByteOrder(QDataStream::LittleEndian);

	if(!checkHeaderType(in,"ttyp",4))
	{
		mapClear();
		qCritical() << "MapInformation::deserializeTerrain - Invalid header.";
		return -1;
	}

	in >> terrainVersion >> numTerrainTypes;
	if(in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeTerrain - Error reading terrain fields.");
		return -1;
	}
	for(i=0;i<numTerrainTypes;i++)
	{
		in >> terrainTypes[i];
	}
	if(in.status()!=QDataStream::Ok)
	{
		mapClear();
		qCritical() << QString("MapInformation::deserializeTerrain - Error reading terrain types.");
		return -1;
	}

	return 0;
}

int MapInformation::deserializeStructures(QDataStream& in)
{
	quint32 u32,i;
	quint32 structVersion;
	quint64 pos; // debug var
	char charArray[60];
	Entity ent;

	if(!checkHeaderType(in,"stru",4))
	{
		m_structures.clear();
		qWarning() << "MapInformation::deserializeStructures - Invalid header.";
		return -1;
	}
	in >> structVersion >> i;

	for(;i>0;i--)
	{
		pos = in.device()->pos();
		if(structVersion <= 19)
		{
			if(in.readRawData(charArray,40)!=40)
			{
				qWarning() << QString("MapInformation::deserializeStructures - Error reading name of structure %1/%2").arg(m_structures.size()).arg(m_structures.size()+i-1);
				m_structures.clear();
				return -1;
			}
		}
		else
		{
			if(in.readRawData(charArray,60)!=60)
			{
				qWarning() << QString("MapInformation::deserializeStructures - Error reading name of structure %1/%2").arg(m_structures.size()).arg(m_structures.size()+i-1);
				m_structures.clear();
				return -1;
			}
		}
		ent.name = QString(charArray);
		in >> ent.id;
		in >> u32;
		ent.position.setX(u32);
		in >> u32;
		ent.position.setY(u32);
		in >> u32;
		ent.position.setZ(u32);
		in >> ent.direction >> ent.player;
		if(in.status()!=QDataStream::Ok)
		{
			qWarning() << QString("MapInformation::deserializeStructures - Error reading fields of structure %1/%2").arg(m_structures.size()).arg(m_structures.size()+i-1);
			m_structures.clear();
			return -1;
		}
		in.skipRawData(14*4);	// Returns bytes skipped or -1 on error
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
		if( structVersion >= 12)
		{
			in.skipRawData(7*4);
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
		if( structVersion >= 14) // Skip visibility
		{
			in.skipRawData(8*1);
		}
		if (structVersion >= 15)
		{
			in.skipRawData(60*1);
			// If version < 20, then this causes no padding, but the short below
			// will still cause two bytes padding; however, if version >= 20, we
			// will cause 4 bytes padding, but the short below will eat 2 of them,
			// leaving us again with only two bytes padding before the next word.
		}
		if (structVersion >= 17)
		{
			in.skipRawData(1*2);
		}
		if (structVersion >= 15)	// structure padding
		{
			in.skipRawData(1*2);
		}
		if (structVersion >= 21)
		{
			in.skipRawData(1*4);
		}
		ent.type = MapObjectTypeStructure;
		if (ent.player > 8/*MAX_PLAYERS*/)
		{
			qWarning() << QString("MapInformation::deserializeStructures - Invalid player field of structure %1/%2").arg(m_structures.size()).arg(m_structures.size()+i-1);
			m_structures.clear();
			return -1;
		}
		if (ent.position.x() >= m_tiles.size()*128/*TILE_WIDTH*/
			|| ent.position.y() >=  m_tiles.at(0).size()*128/*TILE_HEIGHT*/)
		{
			qWarning() << QString("MapInformation::deserializeStructures - Invalid position fields of structure %1/%2").arg(m_structures.size()).arg(m_structures.size()+i-1);
			m_structures.clear();
			return -1;
		}
		if(in.status()!=QDataStream::Ok)
		{
			qWarning() << QString("MapInformation::deserializeStructures - Corrupted or invalid structure file detected (structure %1/%2)").arg(m_structures.size()).arg(m_structures.size()+i-1);
			m_structures.clear();
			return -1;
		}
		m_structures.push_back(ent);
	}

	return 0;
}

int MapInformation::deserializeDroids(QDataStream& in)
{
	quint32 u32,i;
	quint32 droidVersion;
	char charArray[60];
	Entity ent;
	if(!checkHeaderType(in,"dint",4))
	{
		m_droids.clear();
		qWarning() << "MapInformation::deserializeDroids - Invalid header.";
		return -1;
	}

	in >> droidVersion >> i;

	for(;i>0;i--)
	{
		if(droidVersion <= 19)
		{
			if(in.readRawData(charArray,40)!=40)
			{
				qWarning() << QString("MapInformation::deserializeDroids - Error reading name of droid %1/%2").arg(m_droids.size()).arg(m_droids.size()+i-1);
				m_droids.clear();
				return -1;
			}
		}
		else
		{
			if(in.readRawData(charArray,60)!=60)
			{
				qWarning() << QString("MapInformation::deserializeDroids - Error reading name of droid %1/%2").arg(m_droids.size()).arg(m_droids.size()+i-1);
				m_droids.clear();
				return -1;
			}
		}
		ent.name = QString(charArray);
		in >> ent.id;
		in >> u32;
		ent.position.setX(u32);
		in >> u32;
		ent.position.setY(u32);
		in >> u32;
		ent.position.setZ(u32);
		in >> ent.direction >> ent.player;
		if(in.status()!=QDataStream::Ok)
		{
			qWarning() << QString("MapInformation::deserializeDroids - Error reading fields of droid %1/%2").arg(m_droids.size()).arg(m_droids.size()+i-1);
			m_droids.clear();
			return -1;
		}
		in.skipRawData(3*4);

		ent.type = MapObjectTypeDroid;

		// sanity check
		if (ent.position.x() >= m_tiles.size()*128/*TILE_WIDTH*/
			|| ent.position.y() >=  m_tiles.at(0).size()*128/*TILE_HEIGHT*/)
		{
			qWarning() << QString("MapInformation::deserializeDroids - Invalid position fields of droid %1/%2").arg(m_droids.size()).arg(m_droids.size()+i-1);
			m_droids.clear();
			return -1;
		}
		m_droids.push_back(ent);
	}

	foreach(ent,m_droids)
	{
		if (m_playerPresent[ent.player] == false && (ent.name.compare("ConstructorDroid") == 0 || ent.name.compare("ConstructionDroid") == 0))
		{
			m_playerPresent[ent.player] = true;
			m_players++;
		}
	}

	return 0;
}

int MapInformation::deserializeFeats(QDataStream& in)
{
	quint32 featVersion, i;
	quint32 u32;
	Entity ent;
	char charArray[60];

	if(!checkHeaderType(in,"feat",4))
	{
		m_features.clear();
		qWarning() << "MapInformation::deserializeFeats - Invalid header.";
		return -1;
	}

	in >> featVersion >> i;

	for(;i>0;i--)
	{
		if(featVersion <= 19)
		{
			if(in.readRawData(charArray,40)!=40)
			{
				qWarning() << QString("MapInformation::deserializeFeats - Error reading name of feat %1/%2").arg(m_features.size()).arg(m_features.size()+i-1);
				m_features.clear();
				return -1;
			}
		}
		else
		{
			if(in.readRawData(charArray,60)!=60)
			{
				qWarning() << QString("MapInformation::deserializeFeats - Error reading name of feat %1/%2").arg(m_features.size()).arg(m_features.size()+i-1);
				m_features.clear();
				return -1;
			}
		}
		ent.name = QString(charArray);
		in >> ent.id;
		in >> u32;
		ent.position.setX(u32);
		in >> u32;
		ent.position.setY(u32);
		in >> u32;
		ent.position.setZ(u32);
		in >> ent.direction >> ent.player;
		if(in.status()!=QDataStream::Ok)
		{
			qWarning() << QString("MapInformation::deserializeFeats - Error reading fields of feat %1/%2").arg(m_features.size()).arg(m_features.size()+i-1);
			m_features.clear();
			return -1;
		}
		in.skipRawData(3*4);

		if( featVersion >= 14) // Skip visibility
		{
			in.skipRawData(8*1);
		}
		if (ent.position.x() >= m_tiles.size()*128/*TILE_WIDTH*/
			|| ent.position.y() >=  m_tiles.at(0).size()*128/*TILE_HEIGHT*/)
		{
			qWarning() << QString("MapInformation::deserializeFeats - Invalid position fields of feat %1/%2").arg(m_features.size()).arg(m_features.size()+i-1);
			m_features.clear();
			return -1;
		}
		if(in.status()!=QDataStream::Ok)
		{
			qWarning() << QString("MapInformation::deserializeFeats - Corrupted or invalid feature file detected (feat %1/%2)").arg(m_features.size()).arg(m_features.size()+i-1);
			m_features.clear();
			return -1;
		}
		m_features.push_back(ent);
	}
	return 0;
}

void MapInformation::setName(const QString &name)
{
	m_levelName = name;
}

void MapInformation::setSize(const QSize &size)
{
	m_size = size;
}

void MapInformation::setTilesetType(TilesetType tilesetType)
{
	m_tilesetType = tilesetType;
}

QString MapInformation::name()
{
	return m_levelName;
}

QSize MapInformation::size()
{
	return m_size;
}

TilesetType MapInformation::tilesetType()
{
	return m_tilesetType;
}

void MapInformation::mapClear()
{
	for(int i=0;i<8/*MAX_PLAYERS*/;i++)
		m_playerPresent[i] = false;
	m_players = 0;
	m_levelName = "Unnamed";
	m_size = QSize(10, 10);
	m_tilesetType = TilesetTypeArizona;
	m_tiles.clear();
	m_gateways.clear();
}

}
