#ifndef TILESET_H
#define TILESET_H

#include <QtCore/QObject>
#include <QtCore/QStringList>


class QPixmap;

namespace WZMapEditor
{

enum TilesetType
{
	TilesetTypeArizona = 1, // 102
	TilesetTypeUrban = 2, // 222
	TilesetTypeRockies = 3 // 002
};

enum TileType
{
	TileTypeGround = 1,
	TileTypeWater = 2,
	TileTypeRoad = 4,
	TileTypeTracks = 8,
	TileTypeCrater = 16,
	TileTypeCliff = 32,
	TileTypeTransition = 64,
	TileTypeOther = 128,
	TileTypeAll = TileTypeGround | TileTypeWater| TileTypeRoad | TileTypeTracks | TileTypeCrater | TileTypeCliff | TileTypeTransition | TileTypeOther
};

Q_DECLARE_FLAGS(TileTypes, TileType)

struct TileInformation
{
	bool valid;
	bool visible;
	bool ignore;
	int id;
	int category;
	int transitionNorthWest;
	int transitionNorthEast;
	int transitionSouthEast;
	int transitionSouthWest;
	TileType type;
	TilesetType tileset;
	QList<int> backgrounds;
};

class Tileset : public QObject
{
	Q_OBJECT

public:
	Tileset(const QString &fileName, QObject *parent = 0);

	static void load(QObject *parent = NULL);
	static void createCache(TilesetType tileset, int size);
	static QPixmap pixmap(TileInformation tile, int size = -1);
	static QPixmap pixmap(TilesetType tileset, int tile, int size = -1);
	static Tileset* tileset(TilesetType type);
	static QStringList names();
	static TilesetType cachedTileset();
	static int cachedTextureSize();

	TilesetType type();
	QString name();
	TileInformation tile(int tile);
	QStringList categories();
	QList<TileInformation> tiles(bool includeTransitions = true, int category = 0, TileTypes types = TileTypeAll);

protected:
	TileType stringToType(const QString &type);
	QList<int> stringToBackgrounds(const QString &backgrounds);

private:
	TilesetType m_type;
	QString m_name;
	QStringList m_categories;
	QList<QList<int> > m_categoryBackgrounds;
	QList<TileType> m_categoryTypes;
	QList<TileInformation> m_tiles;
};

}

#endif // TILESET_H
