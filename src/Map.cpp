#include "Map.h"

namespace WZMapEditor
{

MapTile::MapTile() : gateway(false), rotation(0), texture(0), height(0), flip(NoFlip)
{
}

MapGateway::MapGateway(int x0, int y0, int x1, int y1)
{
	start = QPoint(x0, y0);
	end = QPoint(x1, y1);
}

Map::Map(QObject *parent) : QObject(parent)
{
	clear();
}

void Map::clear()
{
	m_playerPresent.clear();
	m_playerPresent.resize(MAX_PLAYERS);

	m_levelName = "Unnamed";
	m_size = QSize(10, 10);
	m_tileset = ArizonaTileset;
	m_tiles.clear();
	m_gateways.clear();
	m_modified = true;
}

void Map::setFilePath(const QString &filePath)
{
	m_filePath = filePath;
}

void Map::setName(const QString &name)
{
	m_levelName = name;
}

void Map::setSize(const QSize &size)
{
	m_size = size;
}

void Map::setScrollLimits(const QRect &limits)
{
	m_scrollLimits = limits;
}

void Map::setTime(quint32 time)
{
	m_gameTime = time;
}

void Map::setType(quint32 type)
{
	m_gameType = type;
}

void Map::setTile(const MapTile &tile, int x, int y)
{
	if (x < 0 || y < 0 || x >= m_size.width() || y > m_size.height())
	{
		return;
	}

	m_tiles[x][y] = tile;

	m_modified = true;

	emit modified();
}

void Map::setTileset(TilesetType tilesetType)
{
	m_tileset = tilesetType;
}

void Map::setDroids(QList<MapEntity> droids)
{
	m_droids = droids;
}

void Map::setStructures(QList<MapEntity> structures)
{
	m_structures= structures;
}

void Map::setFeatures(QList<MapEntity> features)
{
	m_features = features;
}

void Map::setTiles(QVector<QVector<MapTile> > tiles)
{
	m_tiles = tiles;
}

void Map::setGateways(QList<MapGateway> gateways)
{
	m_gateways = gateways;
}

void Map::setTerrainTypes(QList<quint16> terrainTypes)
{
	m_terrainTypes = terrainTypes;
}

void Map::setPlayerPresent(QVector<bool> playerPresent)
{
	m_playerPresent = playerPresent;
}

void Map::setModified(bool modified)
{
	m_modified = modified;
}

MapTile Map::getTile(int x, int y)
{
	if (x < 0 || y < 0 || x >= m_tiles.count() || y >= m_tiles.at(x).count())
	{
		return MapTile();
	}
	else
	{
		return m_tiles.at(x).at(y);
	}
}

QString Map::getFilePath()
{
	return m_filePath;
}

QString Map::getName()
{
	return m_levelName;
}

QSize Map::getSize()
{
	return m_size;
}
QRect Map::getScrollLimits()
{
	return m_scrollLimits;
}

quint32 Map::getTime()
{
	return m_gameTime;
}

quint32 Map::getType()
{
	return m_gameType;
}

TilesetType Map::getTileset()
{
	return m_tileset;
}

QVector<QVector<MapTile> > Map::getTiles()
{
	return m_tiles;
}

bool Map::isModified()
{
	return m_modified;
}

}
