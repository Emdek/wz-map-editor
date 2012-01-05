#include "Map2DViewWidget.h"
#include "MapInformation.h"
#include "Tileset.h"
#include "SettingManager.h"

#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QWheelEvent>


namespace WZMapEditor
{

Map2DViewWidget::Map2DViewWidget(QWidget *parent) : QGraphicsView(parent),
	m_mapInformation(NULL),
	m_zoom(100),
	m_tileSize(25)
{
	setMouseTracking(true);
}

void Map2DViewWidget::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() & Qt::CTRL || event->buttons() & Qt::LeftButton)
	{
		setZoom(m_zoom + (event->delta() / 32));

		event->accept();
	}
	else
	{
		QGraphicsView::wheelEvent(event);
	}
}

void Map2DViewWidget::setZoom(qreal zoom)
{
	if (zoom > 500)
	{
		zoom = 500;
	}
	else if (zoom < 10)
	{
		zoom = 10;
	}

	if (m_zoom != zoom)
	{
		const qreal scale = (0.01 * zoom);
		QTransform transform;
		transform.scale(scale, scale);

		setTransform(transform);

		m_zoom = zoom;

		emit zoomChanged(zoom);
	}
}

void Map2DViewWidget::setMapInformation(MapInformation *data)
{
	if (scene())
	{
		scene()->deleteLater();
	}

	setScene(new QGraphicsScene(this));

	m_mapInformation = data;

	const int tileSize = SettingManager::value("tileSize").toInt();

	for (int i = 0; i < m_mapInformation->size().width(); ++i)
	{
		for (int j = 0; j < m_mapInformation->size().height(); ++j)
		{
			MapTile tile(m_mapInformation->tile(i, j));
			QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem(Tileset::pixmap(m_mapInformation->tileset(), tile.texture, tileSize));

			scene()->addItem(pixmap);

			pixmap->scale(((tile.flip & FlipTypeHorizontal)?-1:1), ((tile.flip & FlipTypeVertical)?-1:1));
			pixmap->setRotation(tile.rotation);
			pixmap->setPos((i * tileSize), (j * tileSize));
		}
	}

	connect(m_mapInformation, SIGNAL(changed()), this, SLOT(repaint()));
}

MapInformation* Map2DViewWidget::mapInformation()
{
	return m_mapInformation;
}

int Map2DViewWidget::zoom()
{
	return m_zoom;
}

}
