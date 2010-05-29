#ifndef MAP2DVIEWWIDGET_H
#define MAP2DVIEWWIDGET_H

#include <QtGui/QRubberBand>
#include <QtGui/QWidget>
#include <QtCore/QHash>


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
	int zoom();

public slots:
	void setZoom(qreal zoom);
	void updateSize();
	void updateTextures();

protected:
	void paintEvent(QPaintEvent *event);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	MapInformation* m_mapInformation;
	QHash<int, QPixmap> m_textures;
	QRubberBand *m_rubberBand;
	QPoint m_mousePressStart;
	QPixmap m_mapPixmap;
	QRect m_selection;
	int m_zoom;
	int m_tileSize;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int zoom);
};

}

#endif // MAP2DVIEWWIDGET_H
