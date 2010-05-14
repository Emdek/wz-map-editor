#include "MapInformation.h"


namespace WZMapEditor
{

MapInformation::MapInformation(QObject *parent) : QObject(parent),
	m_size(10, 10)
{
}

void MapInformation::setSize(const QSize &size)
{
	m_size = size;
}

QSize MapInformation::size()
{
	return m_size;
}

}
