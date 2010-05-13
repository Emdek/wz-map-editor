#include "Tileset.h"
#include "SettingManager.h"

#include <QtCore/QDir>
#include <QtCore/QXmlStreamReader>
#include <QtGui/QPixmap>


namespace WZMapEditor
{

Tileset::Tileset(const QString &fileName, QObject *parent) : QObject(parent)
{
	QFileInfo informationPath(fileName);
	QString tilesetName = informationPath.completeBaseName();
	int tileset;

	if (tilesetName == "urban")
	{
		tileset = 2;
	}
	else if (tilesetName == "rockies")
	{
		tileset = 3;
	}
	else
	{
		tileset = 1;
	}

	m_directory = "tertilesc" + QString::number(tileset) + "hw-%1";
	m_categories.append(tr("All"));
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
				m_categoryTypes.append(stringToType(attributes.value("type").toString()));
			}
			else if (reader.name().toString() == "tile")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				TileInformation tile;
				tile.visible = !attributes.hasAttribute("hidden");
				tile.id = attributes.value("id").toString().toInt();
				tile.category = attributes.value("category").toString().toInt();
				tile.transitionNorthWest = tile.category;
				tile.transitionNorthEast = tile.category;
				tile.transitionSouthEast = tile.category;
				tile.transitionSouthWest = tile.category;
				tile.type = (attributes.hasAttribute("type")?stringToType(attributes.value("type").toString()):m_categoryTypes.at(tile.category));

				m_tiles.append(tile);
			}
			else if (reader.name().toString() == "transition")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				TileInformation tile;
				tile.visible = !attributes.hasAttribute("hidden");
				tile.id = attributes.value("id").toString().toInt();
				tile.category = 0;
				tile.transitionNorthWest = attributes.value("northWest").toString().toInt();
				tile.transitionNorthEast = attributes.value("northEast").toString().toInt();
				tile.transitionSouthEast = attributes.value("southEast").toString().toInt();
				tile.transitionSouthWest = attributes.value("southWest").toString().toInt();
				tile.type = TileTypeTransition;

				m_tiles.append(tile);
			}
		}
   }

	file.close();
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

QString Tileset::name()
{
	return m_name;
}

QPixmap Tileset::pixmap(TileInformation tile, int size)
{
	return QPixmap(SettingManager::value("texturesPath").toString() + QDir::separator() + m_directory.arg(size) + QDir::separator() + QString("tile-%1%2.png").arg((tile.id < 10)?QString('0'):QString()).arg(tile.id));
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
