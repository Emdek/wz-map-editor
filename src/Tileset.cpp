#include "Tileset.h"
#include "SettingManager.h"

#include <QtCore/QDir>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QPixmapCache>

namespace WZMapEditor
{

QHash<TilesetType, Tileset*> m_tilesets;
TilesetType m_cachedTileset = ArizonaTileset;
int m_textureSize = 0;

Tileset::Tileset(const QString &fileName, QObject *parent) : QObject(parent)
{
	QFileInfo informationPath(fileName);
	QString tilesetName = informationPath.completeBaseName();

	if (tilesetName == "urban")
	{
		m_type = UrbanTileset;
	}
	else if (tilesetName == "rockies")
	{
		m_type = RockiesTileset;
	}
	else
	{
		m_type = ArizonaTileset;
	}

	CategoryInformation category;
	category.background = 0;
	category.type = OtherTile;
	category.title = tr("All");

	m_categories.append(category);

	QFile file(fileName);
	file.open(QFile::ReadOnly | QFile::Text);

	QXmlStreamReader reader(&file);

	while (!reader.atEnd())
	{
		reader.readNext();

		if (!reader.isStartElement())
		{
			continue;
		}

		if (reader.name().toString() == "tileset")
		{
			QXmlStreamAttributes attributes = reader.attributes();

			m_name = attributes.value("name").toString();
		}
		else if (reader.name().toString() == "category")
		{
			QXmlStreamAttributes attributes = reader.attributes();
			CategoryInformation category;
			category.background = attributes.value("background").toString().toInt();
			category.type = stringToType(attributes.value("type").toString());
			category.title = attributes.value("name").toString();

			m_categories.append(category);
		}
		else if (reader.name().toString() == "tile" || reader.name().toString() == "transition")
		{
			QXmlStreamAttributes attributes = reader.attributes();
			TileInformation tile;
			tile.valid = true;
			tile.visible = !attributes.hasAttribute("hidden");
			tile.ignore = attributes.hasAttribute("ignore");
			tile.id = attributes.value("id").toString().toInt();
			tile.category = attributes.value("category").toString().toInt();
			tile.tileset = m_type;
			tile.background = (attributes.hasAttribute("background") ? attributes.value("background").toString().toInt() : m_categories.at(tile.category).background);

			if (reader.name().toString() == "tile")
			{
				tile.transitionNorthWest = tile.category;
				tile.transitionNorthEast = tile.category;
				tile.transitionSouthEast = tile.category;
				tile.transitionSouthWest = tile.category;
				tile.type = (attributes.hasAttribute("type") ? stringToType(attributes.value("type").toString()) : m_categories.at(tile.category).type);
			}
			else
			{
				tile.transitionNorthWest = attributes.value("northWest").toString().toInt();
				tile.transitionNorthEast = attributes.value("northEast").toString().toInt();
				tile.transitionSouthEast = attributes.value("southEast").toString().toInt();
				tile.transitionSouthWest = attributes.value("southWest").toString().toInt();
				tile.type = TransitionTile;
			}

			m_tiles.append(tile);
		}
	}

	file.close();
}

void Tileset::load(QObject *parent)
{
	QFileInfoList tilesetList = QDir(":/tilesets/").entryInfoList();

	for (int i = 0; i < tilesetList.count(); ++i)
	{
		Tileset *tileset = new Tileset(tilesetList.at(i).absoluteFilePath(), parent);

		m_tilesets[tileset->type()] = tileset;
	}
}

void Tileset::createCache(TilesetType tileset, int size)
{
	QList<TileInformation> tiles = Tileset::tileset(tileset)->tiles();

	QPixmapCache::clear();

	m_cachedTileset = tileset;
	m_textureSize = size;

	for (int i = 0; i < tiles.count(); ++i)
	{
		QPixmapCache::insert(QString::number(tiles.at(i).id), Tileset::pixmap(tileset, tiles.at(i).id, size));
	}
}

QPixmap Tileset::pixmap(TileInformation tile, int size)
{
	if (size == -1)
	{
		size = SettingManager::value("tileSize").toInt();
	}

	if (tile.tileset == m_cachedTileset && size == m_textureSize)
	{
		QPixmap pixmap;

		if (QPixmapCache::find(QString::number(tile.id), &pixmap))
		{
			return pixmap;
		}
	}

	QString fileName = QString("%1texpages%2tertilesc%3hw-%4%5tile-%6%7.png")
		.arg(SettingManager::value("dataPath").toString() + QDir::separator())
		.arg(QDir::separator())
		.arg(static_cast<int>(tile.tileset))
		.arg(size)
		.arg(QDir::separator())
		.arg((tile.id < 10) ? QString('0') : QString())
		.arg(tile.id);

	if (tile.valid && QFile::exists(fileName))
	{
		if (tile.background)
		{
			QPixmap background = pixmap(Tileset::tileset(tile.tileset)->tiles(false, tile.background).first(), size);
			QPainter painter(&background);
			painter.drawPixmap(0, 0, QPixmap(fileName));

			return background;
		}
		else
		{
			return QPixmap(fileName);
		}
	}
	else
	{
		QPixmap debugPixmap(size, size);
		debugPixmap.fill(Qt::red);

		return debugPixmap;
	}
}

QPixmap Tileset::pixmap(TilesetType tileset, int tile, int size)
{
	if (m_tilesets.value(tileset, NULL))
	{
		return pixmap(m_tilesets[tileset]->tile(tile), size);
	}

	return QPixmap();
}

Tileset* Tileset::tileset(TilesetType type)
{
	return m_tilesets.value(type, NULL);
}

QStringList Tileset::names()
{
	QStringList names;

	foreach (Tileset *tileset, m_tilesets)
	{
		names.append(tileset->name());
	}

	return names;
}

TileType Tileset::stringToType(const QString &type)
{
	if (type.isEmpty())
	{
		return GroundTile;
	}

	if (type == "water")
	{
		return WaterTile;
	}

	if (type == "road")
	{
		return RoadTile;
	}

	if (type == "tracks")
	{
		return TracksTile;
	}

	if (type == "crater")
	{
		return CraterTile;
	}

	if (type == "cliff")
	{
		return CliffTile;
	}

	return OtherTile;
}

TilesetType Tileset::type()
{
	return m_type;
}

QString Tileset::name()
{
	return m_name;
}

TileInformation Tileset::tile(int tile)
{
	for (int i = 0; i < m_tiles.count(); ++i)
	{
		if (m_tiles.at(i).id == tile)
		{
			return m_tiles.at(i);
		}
	}

	TileInformation empty;
	empty.valid = false;

	return empty;
}

QStringList Tileset::categories()
{
	QStringList categories;

	for (int i = 0; i < m_categories.count(); ++i)
	{
		categories.append(m_categories.at(i).title);
	}

	return categories;
}

QList<TileInformation> Tileset::tiles(bool includeTransitions, int category, TileTypes types)
{
	QList<TileInformation> tiles;

	for (int i = 0; i < m_tiles.count(); ++i)
	{
		if ((!includeTransitions && !m_tiles.at(i).category && types != TransitionTile) || !m_tiles.at(i).visible)
		{
			continue;
		}

		if (m_tiles.at(i).type & types && (!category || (category > 0 && (m_tiles.at(i).category == category || m_tiles.at(i).transitionNorthWest == category || m_tiles.at(i).transitionNorthEast == category || m_tiles.at(i).transitionSouthEast == category || m_tiles.at(i).transitionSouthWest == category))))
		{
			tiles.append(m_tiles.at(i));
		}
	}

	return tiles;
}

TilesetType Tileset::cachedTileset()
{
	return m_cachedTileset;
}

int Tileset::cachedTextureSize()
{
	return m_textureSize;
}

}
