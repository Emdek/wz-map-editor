#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtWidgets/QGraphicsView>

namespace WZMapEditor
{

class Map;

class Map2DViewWidget : public QGraphicsView
{
	Q_OBJECT

public:
	Map2DViewWidget(QWidget *parent = NULL);

	void setMap(Map *data);
	Map* getMap();
	int getZoom();

public slots:
	void setZoom(qreal getZoom);

protected:
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	Map *m_map;
	int m_zoom;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int getZoom);
};

}

#endif
