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

enum MapObjectType
{
	MapObjectTypeFeature,
	MapObjectTypeStructure,
	MapObjectTypeDroid,
	MapObjectTypeObject
};

enum FlipType
{
	FlipTypeNone = 0,
	FlipTypeVertical = 1,
	FlipTypeHorizontal = 2,
	FlipTypeDiagonal = FlipTypeVertical | FlipTypeHorizontal
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
	void setFilePath(const QString &filePath);
	void setName(const QString &name);
	void setSize(const QSize &size);
	void setScrollLimits(const QRect &limits);
	void setTime(quint32 time);
	void setType(quint32 type);
	void setTileset(TilesetType tilesetType);
	void setDroids(QList<MapEntity> droids);
	void setStructures(QList<MapEntity> structures);
	void setFeatures(QList<MapEntity> features);
	void setTiles(QVector<QVector<MapTile> > tiles);
	void setGateways(QList<MapGateway> gateways);
	void setTerrainTypes(QList<quint16> terrainTypes);
	void setPlayerPresent(QVector<bool> playerPresent);
	void setModified(bool modified);
	MapTile tile(int x, int y);
	QString filePath();
	QString name();
	QSize size();
	QRect scrollLimits();
	quint32 time();
	quint32 type();
	TilesetType tileset();
	QVector<QVector<MapTile> > tiles();
	bool isModified();

private:
	/// Map data
	QSize m_size;
	QVector<QVector<MapTile> > m_tiles;
	QList<MapGateway> m_gateways;

	/// Terrain data
	TilesetType m_tilesetType;

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
	void changed();
};

}

#endif // MAPINFORMATION_H
