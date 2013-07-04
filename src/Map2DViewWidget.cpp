#include "Map2DViewWidget.h"
#include "Map.h"
#include "Tileset.h"
#include "SettingsManager.h"

#include <QtGui/QWheelEvent>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QMenu>

namespace WZMapEditor
{

Map2DViewWidget::Map2DViewWidget(QWidget *parent) : QGraphicsView(parent),
	m_map(NULL),
	m_zoom(100)
{
	setMouseTracking(true);
}

void Map2DViewWidget::mousePressEvent(QMouseEvent *event)
{
	QGraphicsView::mousePressEvent(event);
}

void Map2DViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QGraphicsView::mouseReleaseEvent(event);
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

	const QPoint coordinates = itemAt(event->pos())->data(0).toPoint();

	if (coordinates.isNull())
	{
		return;
	}

	const MapTile tile = m_map->getTile(coordinates.x(), coordinates.y());
	QMenu menu;
	menu.addAction(tr("Tile: %1 %2").arg(coordinates.x()).arg(coordinates.y()));
	menu.addAction(tr("Texture: %1").arg(tile.texture));
	menu.addAction(tr("Rotation: %1").arg(tile.rotation));
	menu.addAction(tr("Flip: %1 %2").arg((tile.flip & HorizontalFlip) ? 'y' : 'n').arg((tile.flip & VerticalFlip) ? 'y' : 'n'));

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

	const qreal tileSize = SettingsManager::getValue("tileSize").toInt();
	const qreal halfTileSize(tileSize / 2);
	const qreal ellipseSize(tileSize / 5);

	scene()->clear();
	scene()->setSceneRect(0, 0, (m_map->getSize().width() * tileSize), (m_map->getSize().height() * tileSize));

	for (int i = 0; i < m_map->getSize().width(); ++i)
	{
		for (int j = 0; j < m_map->getSize().height(); ++j)
		{
			const MapTile tile(m_map->getTile(i, j));
			QTransform transform;
			transform.rotate(tile.rotation);
			transform.scale(((tile.flip & HorizontalFlip) ? -1 : 1), ((tile.flip & VerticalFlip) ? -1 : 1));

			QGraphicsPixmapItem *item = new QGraphicsPixmapItem(Tileset::pixmap(m_map->getTileset(), tile.texture, tileSize));
			item->setOffset(-halfTileSize, -halfTileSize);
			item->setTransform(transform);
			item->setToolTip(tr("Texture: %1").arg(tile.texture));

			scene()->addItem(item);

			item->setData(0, QPoint(i, j));
			item->setPos(((i * tileSize) + halfTileSize), ((m_map->getSize().height() - j - 1) * tileSize) + halfTileSize);
		}
	}

	for (int i = 0; i < m_map->getSize().width(); ++i)
	{
		for (int j = 0; j < m_map->getSize().height(); ++j)
		{
			const int height = m_map->getTile(i, j).height;
			QGraphicsEllipseItem *item = new QGraphicsEllipseItem(((i * tileSize) - (ellipseSize / 2)), (((m_map->getSize().height() - j - 1) * tileSize) - ellipseSize / 2), ellipseSize, ellipseSize);
			item->setBrush(QColor(height, height, height));
			item->setPen(QPen(Qt::NoPen));
			item->setToolTip(tr("Height: %1").arg(height));

			scene()->addItem(item);
		}
	}

	connect(m_map, SIGNAL(modified()), this, SLOT(repaint()));
}

Map* Map2DViewWidget::getMap()
{
	return m_map;
}

int Map2DViewWidget::getZoom()
{
	return m_zoom;
}

}
