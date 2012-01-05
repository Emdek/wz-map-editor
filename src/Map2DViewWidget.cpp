#include "Map2DViewWidget.h"
#include "MapInformation.h"
#include "Tileset.h"
#include "SettingManager.h"

#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QMenu>
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

void Map2DViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
	if (!m_mapInformation || !scene() || !itemAt(event->pos()))
	{
		return;
	}

	QPoint tile = itemAt(event->pos())->data(0).toPoint();
	QMenu menu;
	menu.addAction(tr("Tile: %1 %2").arg(tile.x()).arg(tile.y()));
	menu.addAction(tr("Rotation: %1").arg(m_mapInformation->tile(tile.x(), tile.y()).rotation));
	menu.addAction(tr("Flip: %1 %2").arg((m_mapInformation->tile(tile.x(), tile.y()).flip & FlipTypeHorizontal)?'y':'n').arg((m_mapInformation->tile(tile.x(), tile.y()).flip & FlipTypeVertical)?'y':'n'));

	menu.exec(event->globalPos());
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
	if (!scene())
	{
		setScene(new QGraphicsScene(this));
	}

	m_mapInformation = data;

	const qreal tileSize = SettingManager::value("tileSize").toInt();

	scene()->clear();
	scene()->setSceneRect(0, 0, (m_mapInformation->size().width() * tileSize), (m_mapInformation->size().height() * tileSize));

	for (int i = 0; i < m_mapInformation->size().width(); ++i)
	{
		for (int j = 0; j < m_mapInformation->size().height(); ++j)
		{
			MapTile tile(m_mapInformation->tile(i, j));
			QGraphicsPixmapItem *pixmap = new QGraphicsPixmapItem(Tileset::pixmap(m_mapInformation->tileset(), tile.texture, tileSize));
			pixmap->scale(((tile.flip & FlipTypeHorizontal)?-1:1), ((tile.flip & FlipTypeVertical)?-1:1));
			pixmap->setOffset(((tile.flip & FlipTypeHorizontal)?-tileSize:0), ((tile.flip & FlipTypeVertical)?-tileSize:0));
			pixmap->setTransformOriginPoint((tileSize / 2) + pixmap->offset().x(), (tileSize / 2) + pixmap->offset().y());
			pixmap->setRotation(tile.rotation);

			scene()->addItem(pixmap);

			pixmap->setData(0, QPoint(i, j));
			pixmap->setPos((i * tileSize), ((m_mapInformation->size().height() - j -1) * tileSize));
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
