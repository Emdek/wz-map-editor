#include "Tileset.h"
#include "SettingManager.h"

#include <QtCore/QDir>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QPixmap>


namespace WZMapEditor
{

QHash<TilesetType, Tileset*> m_tilesets;

Tileset::Tileset(const QString &fileName, QObject *parent) : QObject(parent)
{
	QFileInfo informationPath(fileName);
	QString tilesetName = informationPath.completeBaseName();

	if (tilesetName == "urban")
	{
		m_type = TilesetTypeUrban;
	}
	else if (tilesetName == "rockies")
	{
		m_type = TilesetTypeRockies;
	}
	else
	{
		m_type = TilesetTypeArizona;
	}

	m_categories.append(tr("All"));
	m_categoryBackgrounds.append(QList<int>());
	m_categoryTypes.append(TileTypeOther);

	QFile file(fileName);
	file.open(QFile::ReadOnly | QFile::Text);

	QXmlStreamReader reader(file.readAll());

	while (!reader.atEnd())
	{
		reader.readNext();

		if (reader.isStartElement())
		{
			if (reader.name().toString() == "tileset")
			{
				QXmlStreamAttributes attributes = reader.attributes();

				m_name = attributes.value("name").toString();
			}
			else if (reader.name().toString() == "category")
			{
				QXmlStreamAttributes attributes = reader.attributes();

				m_categories.append(attributes.value("name").toString());
				m_categoryBackgrounds.append(stringToBackgrounds(attributes.value("backgrounds").toString()));
				m_categoryTypes.append(stringToType(attributes.value("type").toString()));
			}
			else if (reader.name().toString() == "tile" || reader.name().toString() == "transition")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				TileInformation tile;
				tile.visible = !attributes.hasAttribute("hidden");
				tile.ignore = attributes.hasAttribute("ignore");
				tile.id = attributes.value("id").toString().toInt();
				tile.category = attributes.value("category").toString().toInt();
				tile.backgrounds = (attributes.hasAttribute("backgrounds")?stringToBackgrounds(attributes.value("backgrounds").toString()):m_categoryBackgrounds.at(tile.category));

				if (reader.name().toString() == "tile")
				{
					tile.transitionNorthWest = tile.category;
					tile.transitionNorthEast = tile.category;
					tile.transitionSouthEast = tile.category;
					tile.transitionSouthWest = tile.category;
					tile.type = (attributes.hasAttribute("type")?stringToType(attributes.value("type").toString()):m_categoryTypes.at(tile.category));
				}
				else
				{
					tile.transitionNorthWest = attributes.value("northWest").toString().toInt();
					tile.transitionNorthEast = attributes.value("northEast").toString().toInt();
					tile.transitionSouthEast = attributes.value("southEast").toString().toInt();
					tile.transitionSouthWest = attributes.value("southWest").toString().toInt();
					tile.type = TileTypeTransition;
				}

				m_tiles.append(tile);
			}
		}
	}

	file.close();
}

void Tileset::load(QObject *parent)
{
	QFileInfoList tilesetList = QDir(":/tilesets/").entryInfoList();

	for (int i = 0; i < tilesetList.count(); ++i)
	{
		Tileset * tileset = new Tileset(tilesetList.at(i).absoluteFilePath(), parent);

		m_tilesets[tileset->type()] = tileset;
	}
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
		return TileTypeGround;
	}

	if (type == "water")
	{
		return TileTypeWater;
	}

	if (type == "road")
	{
		return TileTypeRoad;
	}

	if (type == "tracks")
	{
		return TileTypeTracks;
	}

	if (type == "crater")
	{
		return TileTypeCrater;
	}

	if (type == "cliff")
	{
		return TileTypeCliff;
	}

	return TileTypeOther;
}

QList<int> Tileset::stringToBackgrounds(const QString &backgrounds)
{
	QStringList strings = backgrounds.split(',', QString::SkipEmptyParts);
	QList<int> numbers;

	for (int i = 0; i < strings.count(); ++i)
	{
		numbers.append(strings.at(i).toInt());
	}

	return numbers;
}

TilesetType Tileset::type()
{
	return m_type;
}

QString Tileset::name()
{
	return m_name;
}

QPixmap Tileset::pixmap(TileInformation tile, int size)
{
	return QPixmap(QString("%1texpages%2tertilesc%3hw-%4%5tile-%6%7.png").arg(SettingManager::value("dataPath").toString() + QDir::separator()).arg(QDir::separator()).arg(static_cast<int>(m_type)).arg(size).arg(QDir::separator()).arg((tile.id < 10)?QString('0'):QString()).arg(tile.id));
}

QList<QString> Tileset::categories()
{
	return m_categories;
}

QList<TileInformation> Tileset::tiles(bool includeTransitions, int category, TileTypes types)
{
	QList<TileInformation> tiles;

	for (int i = 0; i < m_tiles.count(); ++i)
	{
		if ((!includeTransitions && !m_tiles.at(i).category && types != TileTypeTransition) || !m_tiles.at(i).visible)
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

}
