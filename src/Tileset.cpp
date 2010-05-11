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
			}
			else if (reader.name().toString() == "tile")
			{
				QXmlStreamAttributes attributes = reader.attributes();
				TileInformation tile;
				tile.id = attributes.value("id").toString().toInt();
				tile.category = attributes.value("category").toString().toInt();
				tile.transitionNorth = -1;
				tile.transitionEast = -1;
				tile.transitionSouth = -1;
				tile.transitionWest = -1;
				tile.type = TileTypeNormal;

				m_tiles.append(tile);
			}
		}
   }

	file.close();
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

QList<TileInformation> Tileset::tiles(bool includeTransitions, int category)
{
	QList<TileInformation> tiles;

	for (int i = 0; i < m_tiles.count(); ++i)
	{
		if (!includeTransitions && m_tiles.at(i).category < 0)
		{
			continue;
		}

		if (category < 0 || (category >= 0 && (m_tiles.at(i).category == category || (includeTransitions && (m_tiles.at(i).transitionNorth == category || m_tiles.at(i).transitionEast == category || m_tiles.at(i).transitionSouth == category || m_tiles.at(i).transitionWest == category)))))
		{
			tiles.append(m_tiles.at(i));
		}
	}

	return m_tiles;
}

}
