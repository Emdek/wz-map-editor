#include "MapInformation.h"


namespace WZMapEditor
{

MapTile::MapTile() : gateway(false), rotation(0), texture(0), height(0), flip(FlipTypeNone)
{
}

Gateway::Gateway(int x0, int y0, int x1, int y1)
{
	start = QPoint(x0, y0);
	end = QPoint(x1, y1);
}

MapInformation::MapInformation(QObject *parent) : QObject(parent)
{
	clear();
}

void MapInformation::clear()
{
	m_playerPresent.clear();
	m_playerPresent.resize(MAX_PLAYERS);

	m_levelName = "Unnamed";
	m_size = QSize(10, 10);
	m_tilesetType = TilesetTypeArizona;
	m_tiles.clear();
	m_gateways.clear();
	m_modified = true;
}

void MapInformation::setFilePath(const QString &filePath)
{
	m_filePath = filePath;
}

void MapInformation::setName(const QString &name)
{
	m_levelName = name;
}

void MapInformation::setSize(const QSize &size)
{
	m_size = size;
}

void MapInformation::setScrollLimits(const QRect &limits)
{
	m_scrollLimits = limits;
}

void MapInformation::setTime(quint32 time)
{
	m_gameTime = time;
}

void MapInformation::setType(quint32 type)
{
	m_gameType = type;
}

void MapInformation::setTileset(TilesetType tilesetType)
{
	m_tilesetType = tilesetType;
}

void MapInformation::setDroids(QList<Entity> droids)
{
	m_droids = droids;
}

void MapInformation::setStructures(QList<Entity> structures)
{
	m_structures= structures;
}

void MapInformation::setFeatures(QList<Entity> features)
{
	m_features = features;
}

void MapInformation::setTiles(QVector<QVector<MapTile> > tiles)
{
	m_tiles = tiles;
}

void MapInformation::setGateways(QList<Gateway> gateways)
{
	m_gateways = gateways;
}

void MapInformation::setTerrainTypes(QList<quint8> terrainTypes)
{
	m_terrainTypes = terrainTypes;
}

void MapInformation::setPlayerPresent(QVector<bool> playerPresent)
{
	m_playerPresent = playerPresent;
}

void MapInformation::setModified(bool modified)
{
	m_modified = modified;
}

MapTile MapInformation::tile(int x, int y)
{
	if (x >= m_tiles.count() || y >= m_tiles.at(x).count())
	{
		return MapTile();
	}
	else
	{
		return m_tiles.at(x).at(y);
	}
}

QString MapInformation::filePath()
{
	return m_filePath;
}

QString MapInformation::name()
{
	return m_levelName;
}

QSize MapInformation::size()
{
	return m_size;
}
QRect MapInformation::scrollLimits()
{
	return m_scrollLimits;
}

quint32 MapInformation::time()
{
	return m_gameTime;
}

quint32 MapInformation::type()
{
	return m_gameType;
}

TilesetType MapInformation::tileset()
{
	return m_tilesetType;
}

QVector<QVector<MapTile> > MapInformation::tiles()
{
	return m_tiles;
}

bool MapInformation::isModified()
{
	return m_modified;
}

}
