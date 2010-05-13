#ifndef TILESET_H
#define TILESET_H

#include <QtCore/QObject>
#include <QtCore/QStringList>


class QPixmap;

namespace WZMapEditor
{

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
	bool visible;
	bool ignore;
	int id;
	int category;
	int transitionNorthWest;
	int transitionNorthEast;
	int transitionSouthEast;
	int transitionSouthWest;
	TileType type;
	QList<int> backgrounds;
};

class Tileset : public QObject
{
	Q_OBJECT

public:
	Tileset(const QString &fileName, QObject *parent = 0);

	QString name();
	QPixmap pixmap(TileInformation tile, int size = 64);
	QList<QString> categories();
	QList<TileInformation> tiles(bool includeTransitions, int category = 0, TileTypes types = TileTypeAll);

protected:
	TileType stringToType(const QString &type);

private:
	QString m_name;
	QString m_directory;
	QStringList m_categories;
	QList<TileType> m_categoryTypes;
	QList<TileInformation> m_tiles;
};

}

#endif // TILESET_H
