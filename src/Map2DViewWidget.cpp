#include "Map2DViewWidget.h"
#include "Map.h"
#include "Tileset.h"
#include "SettingManager.h"

#include <QtGui/QGraphicsPixmapItem>
#include <QtGui/QMenu>
#include <QtGui/QWheelEvent>


namespace WZMapEditor
{

Map2DViewWidget::Map2DViewWidget(QWidget *parent) : QGraphicsView(parent),
	m_map(NULL),
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
	if (!m_map || !scene() || !itemAt(event->pos()))
	{
		return;
	}

	const QPoint item = itemAt(event->pos())->data(0).toPoint();
	const MapTile tile = m_map->tile(item.x(), item.y());
	QMenu menu;
	menu.addAction(tr("Tile: %1 %2").arg(item.x()).arg(item.y()));
	menu.addAction(tr("Texture: %1").arg(tile.texture));
	menu.addAction(tr("Rotation: %1").arg(tile.rotation));
	menu.addAction(tr("Flip: %1 %2").arg((tile.flip & HorizontalFlip)?'y':'n').arg((tile.flip & VerticalFlip)?'y':'n'));

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

void Map2DViewWidget::setMap(Map *data)
{
	if (!scene())
	{
		setScene(new QGraphicsScene(this));
	}

	m_map = data;

	const qreal tileSize = SettingManager::value("tileSize").toInt();
	const qreal halfTileSize(tileSize / 2);

	scene()->clear();
	scene()->setSceneRect(0, 0, (m_map->size().width() * tileSize), (m_map->size().height() * tileSize));

	for (int i = 0; i < m_map->size().width(); ++i)
	{
		for (int j = 0; j < m_map->size().height(); ++j)
		{
			const MapTile tile(m_map->tile(i, j));
			QGraphicsPixmapItem *item = new QGraphicsPixmapItem(Tileset::pixmap(m_map->tileset(), tile.texture, tileSize));
			item->setOffset(-halfTileSize, -halfTileSize);
			item->rotate(tile.rotation);
			item->scale(((tile.flip & HorizontalFlip)?-1:1), ((tile.flip & VerticalFlip)?-1:1));

			scene()->addItem(item);

			item->setData(0, QPoint(i, j));
			item->setPos((i * tileSize) + halfTileSize, ((m_map->size().height() - j -1) * tileSize) + halfTileSize);
		}
	}

	connect(m_map, SIGNAL(changed(QRect)), this, SLOT(repaint()));
}

Map* Map2DViewWidget::map()
{
	return m_map;
}

int Map2DViewWidget::zoom()
{
	return m_zoom;
}

}
