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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Map3DViewWidget::resizeGL(int width, int height)
{
//	int size = qMin(width, height);
//	glViewport((width - size) / 2, (height - size) / 2, size, size);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 1500.0f );
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

//	glEnable (GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glTranslatef(0.0f, 0.0f, ((1.0 / m_zoom) * -2000));
	glRotatef(m_rotationX / 4.0f, 1.0, 0.0, 0.0);
	glRotatef(m_rotationY / 4.0f, 0.0, 1.0, 0.0);
	glRotatef(m_rotationZ / 4.0f, 0.0, 0.0, 1.0);
	glTranslatef(this->m_offsetX * 0.2f, m_offsetY * 0.2f, 0.0f);

	if (!m_mapInformation)
	{
		return;
	}

	// this moves objects drawing position
	// simple hack to set center in really center of map - not on left-bottom edge
	int centerFactorX = (m_mapInformation->size().width()  * 1.0f) / 2;
	int centerFactorY = (m_mapInformation->size().height() * 1.0f) / 2;

	for (int i = 1; i <= m_mapInformation->size().width(); ++i)
	{
		for (int j = 1; j <= m_mapInformation->size().height(); ++j)
		{
			QPointF coordinates(((1.0f * i) - centerFactorX), ((1.0f * j) - centerFactorY));

			bindTexture(m_textures[m_mapInformation->tile((i - 1), (j - 1)).texture]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBegin(GL_QUADS);

//			glColor3f(0.0f, 0.0f, 1.0f);

			glNormal3f(coordinates.x(), (coordinates.y() - 1.0f), 0.0f);
			glTexCoord2f(1, 0);
			glVertex3f(coordinates.x(), (coordinates.y() - 1.0f), 0.0f);

//			glColor3f(1.0f, 0.0f, 0.0f);

			glNormal3f(coordinates.x(), coordinates.y(), 0.0f);
			glTexCoord2f(0, 0);
			glVertex3f(coordinates.x(), coordinates.y(), 0.0f);

//			glColor3f(0.0f, 1.0f, 0.0f);

			glNormal3f((coordinates.x() - 1.0f), coordinates.y(),  0.0f);
			glTexCoord2f(0, 1);
			glVertex3f((coordinates.x() - 1.0f), coordinates.y(),  0.0f);

//			glColor3f(1.0f, 0.0f, 0.0f);

			glNormal3f((coordinates.x() - 1.0f), (coordinates.y() - 1.0f),  0.0f);
			glTexCoord2f(1, 1);
			glVertex3f((coordinates.x() - 1.0f), (coordinates.y() - 1.0f),  0.0f);

			glEnd();

/*			glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f * i - centerFactorX - 1.0f, 1.0f * j - centerFactorY, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f * i - centerFactorX - 1.0f, 1.0f * j - centerFactorY - 1.0f, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f * i - centerFactorX, 1.0f * j - centerFactorY - 1.0f, 0.0f);
			glEnd();*/
		}
	}
}

void Map3DViewWidget::wheelEvent(QWheelEvent *event)
{
	if (event->modifiers() & Qt::CTRL || event->buttons() & Qt::LeftButton)
	{
		setZoom(m_zoom + (event->delta() / 32));
	}
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
	if (!m_mapInformation || m_mapInformation->tileset() != data->tileset())
	{
		updateTextures();
	}

	m_mapInformation = data;

	repaint();

	connect(m_mapInformation, SIGNAL(changed()), this, SLOT(repaint()));
}

void Map3DViewWidget::moveLeft()
{
	--m_offsetX;

	repaint();
}

void Map3DViewWidget::moveRight()
{
	++m_offsetX;

	repaint();
}

void Map3DViewWidget::moveUp()
{
	++m_offsetY;

	repaint();
}

void Map3DViewWidget::moveDown()
{
	--m_offsetY;

	repaint();
}

void Map3DViewWidget::updateTextures()
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
