#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtGui/QGraphicsView>


namespace WZMapEditor
{

class MapInformation;

class Map2DViewWidget : public QGraphicsView
{
	Q_OBJECT

public:
	Map2DViewWidget(QWidget *parent = NULL);

	void setMapInformation(MapInformation *data);
	MapInformation* mapInformation();
	int zoom();

public slots:
	void setZoom(qreal zoom);

protected:
	void wheelEvent(QWheelEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	MapInformation *m_mapInformation;
	int m_zoom;
	int m_tileSize;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int zoom);
};

}

#endif // MAP2DVIEWWIDGET_H
