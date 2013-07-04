#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H

#include "Tileset.h"

#include <QtCore/QObject>
#include <QtCore/QPoint>
#include <QtCore/QSize>
#include <QtCore/QVector>
#include <QtCore/QRect>
#include <QtGui/QVector3D>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QDataStream>
#include <QtCore/QFileInfo>

namespace WZMapEditor
{

const quint32 MAX_PLAYERS = 8;
const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 128;
const int MAX_MAPAREA = (256 * 256);
const quint8 MAX_TILE_TEXTURES = 255;

enum EntityType
{
	FeatureEntity,
	StructureEntity,
	DroidEntity,
	ObjectEntity,
	TerrainEntity
};

enum FlipType
{
	NoFlip = 0,
	VerticalFlip = 1,
	HorizontalFlip = 2,
	DiagonalFlip = VerticalFlip | HorizontalFlip
};

Q_DECLARE_FLAGS(FlipTypes, FlipType)

struct MapTile
{
	MapTile();
	bool gateway;
	int rotation;
	int texture;
	int height;
	FlipTypes flip;
	QPoint position;
};

struct MapGateway
{
	MapGateway(){}
	MapGateway(int x0, int y0, int x1, int y1);
	QPoint start;
	QPoint end;
};

struct MapEntity
{
	quint32 id;
	quint32 player;
	int type;	// "IMD" LND object type
	QString name;	//char name[128]
	QString script;	// char	script[32];
	QVector3D position;
	quint32 direction;
};

class Map : public QObject
{
	Q_OBJECT

public:
	Map(QObject *parent = NULL);

	int deserialize(const QFileInfo& fileNfo);
	void clear();
	void setFilePath(const QString &getFilePath);
	void setName(const QString &getName);
	void setSize(const QSize &getSize);
	void setScrollLimits(const QRect &limits);
	void setTime(quint32 getTime);
	void setType(quint32 getType);
	void setTile(const MapTile &getTile, int x, int y);
	void setTileset(TilesetType tilesetType);
	void setDroids(QList<MapEntity> droids);
	void setStructures(QList<MapEntity> structures);
	void setFeatures(QList<MapEntity> features);
	void setTiles(QVector<QVector<MapTile> > getTiles);
	void setGateways(QList<MapGateway> gateways);
	void setTerrainTypes(QList<quint16> terrainTypes);
	void setPlayerPresent(QVector<bool> playerPresent);
	void setModified(bool modified);
	MapTile getTile(int x, int y);
	QString getFilePath();
	QString getName();
	QSize getSize();
	QRect getScrollLimits();
	quint32 getTime();
	quint32 getType();
	TilesetType getTileset();
	QVector<QVector<MapTile> > getTiles();
	bool isModified();

private:
	/// Map data
	QSize m_size;
	QVector<QVector<MapTile> > m_tiles;
	QList<MapGateway> m_gateways;

	/// Terrain data
	TilesetType m_tileset;

	/* LUT that returns the terrain type of a given tile texture */
	QList<quint16> m_terrainTypes;

	/// Game data
	QRect m_scrollLimits;
	quint32 m_gameTime;
	quint32 m_gameType;
	QString m_levelName;
	quint32 m_playerPower[MAX_PLAYERS];

	/** Entity data:
	  * Structures, droids and features
	  */
	QList<MapEntity> m_droids;
	QList<MapEntity> m_structures;
	QList<MapEntity> m_features;

	QString m_filePath;

	QVector<bool> m_playerPresent;
	bool m_modified;

signals:
	void modified();
};

}

#endif
