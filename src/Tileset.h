#ifndef TILESET_H
#define TILESET_H

#include <QtCore/QObject>
#include <QtCore/QStringList>


class QPixmap;

namespace WZMapEditor
{

enum TileType
{
	TileTypeNormal = 0,
	TileTypeWater,
	TileTypeRoad,
	TileTypeTracks
};

struct TileInformation
{
	int id;
	int category;
	int transitionNorth;
	int transitionEast;
	int transitionSouth;
	int transitionWest;
	TileType type;
};

class Tileset : public QObject
{
	Q_OBJECT

public:
	Tileset(const QString &fileName, QObject *parent = 0);

	QString name();
	QPixmap pixmap(TileInformation tile, int size = 64);
	QList<QString> categories();
	QList<TileInformation> tiles(bool includeTransitions, int category = -1);

private:
	QString m_name;
	QString m_directory;
	QStringList m_categories;
	QList<TileInformation> m_tiles;
};

}

#endif // TILESET_H
