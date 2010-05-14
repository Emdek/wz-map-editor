#include "MapInformation.h"


namespace WZMapEditor
{

MapInformation::MapInformation(QObject *parent) : QObject(parent),
	m_size(10, 10),
	m_tilesetType(TilesetTypeArizona)
{
}

void MapInformation::setSize(const QSize &size)
{
	m_size = size;
}

void MapInformation::setTilesetType(TilesetType tilesetType)
{
	m_tilesetType = tilesetType;
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
