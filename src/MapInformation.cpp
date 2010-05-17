#include "MapInformation.h"


namespace WZMapEditor
{

MapInformation::MapInformation(QObject *parent) : QObject(parent),
	m_name("Unnamed"),
	m_size(10, 10),
	m_tilesetType(TilesetTypeArizona)
{
}

void MapInformation::setName(const QString &name)
{
	m_name = name;
}

void MapInformation::setSize(const QSize &size)
{
	m_size = size;
}

void MapInformation::setTilesetType(TilesetType tilesetType)
{
	m_tilesetType = tilesetType;
}

QString MapInformation::name()
{
	return m_name;
}

QSize MapInformation::size()
{
	return m_size;
}

TilesetType MapInformation::tilesetType()
{
	return m_tilesetType;
}

}
