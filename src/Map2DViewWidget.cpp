#include "Map2DViewWidget.h"
#include "MapInformation.h"


namespace WZMapEditor
{

Map2DViewWidget::Map2DViewWidget(QWidget *parent) : QWidget(parent)
{
}

void Map2DViewWidget::setMapInformation(MapInformation *data)
{
	m_mapInformation = data;
}

MapInformation* Map2DViewWidget::mapInformation()
{
	return m_mapInformation;
}

}
