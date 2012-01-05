#include "Map3DViewWidget.h"
#include "MapInformation.h"
#include "Tileset.h"
#include "SettingManager.h"

#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>

#include <cstdio>

namespace WZMapEditor
{

Map3DViewWidget::Map3DViewWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
	m_mapInformation(NULL),
	m_moving(false),
	m_rotating(false),
	m_rotationX(0),
	m_rotationY(0),
	m_rotationZ(0),
	m_offsetX(0),
	m_offsetY(0),
	m_zoom(50)
{
	setMouseTracking(true);

	QAction *moveLeft = new QAction(this);
	QAction *moveRight = new QAction(this);
	QAction *moveUp = new QAction(this);
	QAction *moveDown = new QAction(this);

	moveLeft->setShortcut(Qt::Key_Left);
	moveRight->setShortcut(Qt::Key_Right);
	moveUp->setShortcut(Qt::Key_Up);
	moveDown->setShortcut(Qt::Key_Down);

	addAction(moveLeft);
	addAction(moveRight);
	addAction(moveUp);
	addAction(moveDown);

	connect(moveLeft, SIGNAL(triggered()), this, SLOT(moveLeft()));
	connect(moveRight, SIGNAL(triggered()), this, SLOT(moveRight()));
	connect(moveUp, SIGNAL(triggered()), this, SLOT(moveUp()));
	connect(moveDown, SIGNAL(triggered()), this, SLOT(moveDown()));

	setFocus();
}

void Map3DViewWidget::initializeGL()
{
	qglClearColor(QColor(0, 0, 32, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glHint(GL_PERSPECTIVE_CORECTION_HINT, GL_NICEST);

	glColor4f(1.0, 1.0, 1.0, 1.0);
}

void Map3DViewWidget::resizeGL(int width, int height)
{
//	int size = qMin(width, height);
//	glViewport((width - size) / 2, (height - size) / 2, size, size);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 1500.0);
/*
#ifdef QT_OPENGL_ES_1
		glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
		glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
*/
	glMatrixMode(GL_MODELVIEW);
	paintGL();
}

void Map3DViewWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

//	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(0.0, 0.0, ((1.0 / m_zoom) * -2000));
	glRotatef(m_rotationX / 4.0, 1.0, 0.0, 0.0);
	glRotatef(m_rotationY / 4.0, 0.0, 1.0, 0.0);
	glRotatef(m_rotationZ / 4.0, 0.0, 0.0, 1.0);
	glTranslatef(m_offsetX * 0.2, m_offsetY * 0.2, 0.0);

	if (!m_mapInformation)
	{
		return;
	}

	// this moves objects drawing position
	// simple hack to set center in really center of map - not on left-bottom edge
	int centerFactorX = (m_mapInformation->size().width()  * 1.0) / 2;
	int centerFactorY = (m_mapInformation->size().height() * 1.0) / 2;

	glMatrixMode(GL_TEXTURE);

	const int tileSize = SettingManager::value("tileSize").toInt();

	for (int i = 1; i <= m_mapInformation->size().width(); ++i)
	{
		for (int j = 1; j <= m_mapInformation->size().height(); ++j)
		{
			QPointF coordinates(((1.0 * i) - centerFactorX), ((1.0 * j) - centerFactorY));
			MapTile tile(m_mapInformation->tile((i - 1), (j - 1)));

			glPushMatrix();

			glScalef(((tile.flip & FlipTypeHorizontal) ? -1.0 : 1.0), ((tile.flip & FlipTypeVertical) ? -1.0 : 1.0), 0.0);
			glRotatef((GLfloat)(tile.rotation + 90) * 1.0, 0.0, 0.0, 1.0);

			bindTexture(Tileset::pixmap(m_mapInformation->tileset(), tile.texture, tileSize));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			float tile_left_top, tile_left_bottom, tile_right_top, tile_right_bottom;


			MapTile t_left_top(m_mapInformation->tile((i - 1), (j - 1)));
			tile_left_top = (t_left_top.height * 3.0f) / 255.0f;

			MapTile t_right_top(m_mapInformation->tile((i), (j - 1)));
			tile_right_top = (t_right_top.height * 3.0f) / 255.0f;

			if (j - 2 < 0)
				tile_left_bottom = tile_left_top;
			else
			{
				MapTile t_left_bottom(m_mapInformation->tile((i - 1), (j - 2)));
				tile_left_bottom = (t_left_bottom.height * 3.0f) / 255.0f;
			}

			if (j - 2 < 0)
				tile_right_bottom = tile_right_top;
			else
			{
				MapTile t_right_bottom(m_mapInformation->tile((i), (j - 2)));
				tile_right_bottom = (t_right_bottom.height * 3.0f) / 255.0f;
			}

			float posY = 1.0 * j - centerFactorY;
			float posX = 1.0 * i - centerFactorX;

			glBegin(GL_TRIANGLES);
			glTexCoord2f(0, 0);
			glVertex3f(posX, posY - 1.0, tile_right_bottom);

			glTexCoord2f(1, 0);
			glVertex3f(posX, posY, tile_right_top);

			glTexCoord2f(1, 1);
			glVertex3f(posX - 1.0, posY, tile_left_top);
			glEnd();

			glBegin(GL_TRIANGLES);
			glTexCoord2f(1, 1);
			glVertex3f(posX - 1.0, posY, tile_left_top);

			glTexCoord2f(0, 1);
			glVertex3f(posX - 1.0, posY - 1.0, tile_left_bottom);

			glTexCoord2f(0, 0);
			glVertex3f(posX, posY - 1.0, tile_right_bottom);
			glEnd();
			glPopMatrix();
		}
	}

	glMatrixMode(GL_MODELVIEW);
}

void Map3DViewWidget::wheelEvent(QWheelEvent *event)
{
//	if (event->modifiers() & Qt::CTRL || event->buttons() & Qt::LeftButton)
//	{
		setZoom(m_zoom + (event->delta() / 32));
/*	}
	else
	{
		int move = (event->delta() / 32);

		if (event->modifiers() & Qt::ALT)
		{
			m_offsetX -= move;
		}
		else
		{
			m_offsetY += move;
		}

		repaint();
	}
*/
	event->accept();
}

void Map3DViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = true;

		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void Map3DViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = false;

		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void Map3DViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	static int lastMoveX, lastMoveY;

	if (m_moving == true)
	{
		m_rotationZ -= (lastMoveX - event->pos().x());
		m_rotationX -= (lastMoveY - event->pos().y());

		repaint();
	}

	lastMoveX = event->pos().x();
	lastMoveY = event->pos().y();

	emit cooridantesChanged(event->pos().x(), event->pos().y(), 0);
}

void Map3DViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu;

	QAction *openAct = new QAction("not yet implemented", this);

	menu.addAction(openAct);
	menu.exec(mapToGlobal(event->pos()));

	menu.removeAction(openAct);
	delete openAct;
}

void Map3DViewWidget::setZoom(qreal zoom)
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

		repaint();

		emit zoomChanged(zoom);
	}
}

void Map3DViewWidget::setMapInformation(MapInformation *data)
{
	m_mapInformation = data;

	repaint();

	connect(m_mapInformation, SIGNAL(changed()), this, SLOT(repaint()));
}

void Map3DViewWidget::moveLeft()
{
	m_offsetX += 5;

	repaint();
}

void Map3DViewWidget::moveRight()
{
	m_offsetX -= 5;

	repaint();
}

void Map3DViewWidget::moveUp()
{
	m_offsetY -= 5;

	repaint();
}

void Map3DViewWidget::moveDown()
{
	m_offsetY += 5;

	repaint();
}

MapInformation* Map3DViewWidget::mapInformation()
{
	return m_mapInformation;
}

QSize Map3DViewWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize Map3DViewWidget::sizeHint() const
{
	return QSize(320, 240);
}

int Map3DViewWidget::zoom()
{
	return m_zoom;
}

}
