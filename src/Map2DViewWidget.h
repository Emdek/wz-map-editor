#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtGui/QGraphicsView>


namespace WZMapEditor
{

class Map;

class Map2DViewWidget : public QGraphicsView
{
	Q_OBJECT

public:
	Map2DViewWidget(QWidget *parent = NULL);

	void setMap(Map *data);
	Map* map();
	int zoom();

public slots:
	void setZoom(qreal zoom);

protected:
	void wheelEvent(QWheelEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	Map *m_map;
	int m_zoom;
	int m_tileSize;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int zoom);
};

}

#endif // MAP2DVIEWWIDGET_H
