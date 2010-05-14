#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtGui/QWidget>


namespace WZMapEditor
{

class MapInformation;

class Map2DViewWidget : public QWidget
{
	Q_OBJECT

public:
	Map2DViewWidget(QWidget *parent = NULL);

	void setMapInformation(MapInformation *data);
	MapInformation* mapInformation();

private:
	MapInformation* m_mapInformation;
};

}

#endif // MAP2DVIEWWIDGET_H
