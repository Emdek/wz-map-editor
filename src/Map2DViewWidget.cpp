#include "Map2DViewWidget.h"
#include "MapInformation.h"
#include "Tileset.h"
#include "SettingManager.h"

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>
#include <QtGui/QStyle>


#include <QtCore/QTime>
#include <QDebug>


namespace WZMapEditor
{

Map2DViewWidget::Map2DViewWidget(QWidget *parent) : QWidget(parent),
	m_mapInformation(NULL),
	m_rubberBand(NULL),
	m_zoom(100),
	m_tileSize(25)
{
	setMouseTracking(true);
	setAttribute(Qt::WA_OpaquePaintEvent);
	updateSize();
}

void Map2DViewWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event)

	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	if (m_mapPixmap.isNull() && m_mapInformation)
	{
		m_mapPixmap = QPixmap(size());

		QPainter mapPainter(&m_mapPixmap);
		mapPainter.fillRect(rect(), Qt::red);
		mapPainter.setCompositionMode(QPainter::CompositionMode_Source);
		mapPainter.setRenderHint(QPainter::Antialiasing, true);

		QTime test;
		test.start();

		for (int i = 0; i < m_mapInformation->size().width(); ++i)
		{
			for (int j = 0; j < m_mapInformation->size().height(); ++j)
			{
				QRect tile((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize);

				mapPainter.drawPixmap(tile, m_textures[m_mapInformation->tile(i, j).texture]);
			}
		}

		qDebug() << "repaint time" << test.elapsed();
	}

	painter.drawPixmap(0, 0, m_mapPixmap);

	QRect selection(m_selection);
	selection = QRect((selection.topLeft() * m_tileSize), (selection.size() * m_tileSize));

	if (m_mapInformation && m_rubberBand)
	{
		for (int i = 0; i < m_mapInformation->size().width(); ++i)
		{
			for (int j = 0; j < m_mapInformation->size().height(); ++j)
			{
				QRect tile((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize);

				if (tile.intersects(m_rubberBand->geometry()) || (selection.isValid() && tile.intersects(selection)))
				{
					QStyleOptionViewItemV4 option;
					option.initFrom(this);
					option.rect = tile;
					option.state |= QStyle::State_Selected;

					if (!hasFocus())
					{
						option.state &= ~QStyle::State_Active;
					}

					painter.setOpacity(0.5);

					style()->drawPrimitive(QStyle::PE_PanelItemViewItem, &option, &painter, this);

					painter.setOpacity(1);
				}
			}
		}
	}
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
		event->ignore();
	}
}

void Map2DViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (!m_mapInformation)
	{
		return;
	}

	m_mousePressStart = event->pos();
	m_selection = QRect();

	for (int i = 0; i < m_mapInformation->size().width(); ++i)
	{
		for (int j = 0; j < m_mapInformation->size().height(); ++j)
		{
			if (QRect((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize).contains(event->pos()))
			{
				m_selection = QRect(i, j, 1, 1);

				break;
			}
		}
	}

	if (event->buttons() & Qt::LeftButton)
	{
		if (!m_rubberBand)
		{
			m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
		}

		m_rubberBand->setGeometry(QRect(m_mousePressStart, QSize()));
		m_rubberBand->show();

		repaint();
	}
}

void Map2DViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_rubberBand && m_rubberBand->isVisible())
	{
		if (m_mousePressStart != event->pos() && m_mapInformation)
		{
			m_selection = QRect();

			for (int i = 0; i < m_mapInformation->size().width(); ++i)
			{
				for (int j = 0; j < m_mapInformation->size().height(); ++j)
				{
					if (m_rubberBand->geometry().intersects(QRect((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize)))
					{
						m_selection |= QRect(i, j, 1, 1);
					}
				}
			}
		}

		m_rubberBand->hide();

		repaint();
	}
}

void Map2DViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_mapInformation)
	{
		if (event->buttons() & Qt::LeftButton  && m_rubberBand && m_rubberBand->isVisible())
		{
			m_selection = QRect();

			for (int i = 0; i < m_mapInformation->size().width(); ++i)
			{
				for (int j = 0; j < m_mapInformation->size().height(); ++j)
				{
					if (m_rubberBand->geometry().intersects(QRect((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize)))
					{
						m_selection |= QRect(i, j, 1, 1);
					}
				}
			}

			m_rubberBand->setGeometry(QRect(m_mousePressStart, event->pos()).normalized());

			repaint();
		}

		for (int i = 0; i < m_mapInformation->size().width(); ++i)
		{
			for (int j = 0; j < m_mapInformation->size().height(); ++j)
			{
				if (QRect((i * m_tileSize), (j * m_tileSize), m_tileSize, m_tileSize).contains(event->pos()))
				{
					emit cooridantesChanged(i, j, 0);
				}
			}
		}
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
		m_zoom = zoom;

		updateSize();

		emit zoomChanged(zoom);
	}
}

void Map2DViewWidget::setMapInformation(MapInformation *data)
{
	const bool updateTextures = (!m_mapInformation || m_mapInformation->tileset() != data->tileset());

	m_mapInformation = data;

	if (updateTextures)
	{
		this->updateTextures();
	}

	m_mapPixmap = QPixmap();

	updateSize();

	connect(m_mapInformation, SIGNAL(changed()), this, SLOT(repaint()));
}

void Map2DViewWidget::updateSize()
{
	QSize size;

	m_tileSize = (m_zoom / 2);

	if (m_mapInformation)
	{
		size = (m_mapInformation->size() * m_tileSize);
	}
	else
	{
		size = (QSize(10, 10) * m_tileSize);
	}

	if (size != this->size())
	{
		m_mapPixmap = QPixmap();

		setMinimumSize(size);
		setMaximumSize(size);
	}
}

void Map2DViewWidget::updateTextures()
{
	const TilesetType tileset = (m_mapInformation?m_mapInformation->tileset():TilesetTypeArizona);
	const int tileSize = SettingManager::value("tileSize").toInt();
	QList<TileInformation> tiles = Tileset::tileset(tileset)->tiles();

	m_textures.clear();

	for (int i = 0; i < tiles.count(); ++i)
	{
		m_textures[tiles.at(i).id] = Tileset::pixmap(tileset, tiles.at(i).id, tileSize);
	}
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
