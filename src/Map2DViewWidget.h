#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtGui/QRubberBand>
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

public slots:
	void setZoom(qreal zoom);

protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	MapInformation* m_mapInformation;
	QRubberBand *m_rubberBand;
	QPoint m_mousePressStart;
	QRect m_selection;
	qreal m_zoom;
	int m_tileSize;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(qreal zoom);
};

}

#endif // MAP2DVIEWWIDGET_H
