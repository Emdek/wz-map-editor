#include "Map3DViewWidget.h"
#include "MapInformation.h"
#include "Tileset.h"

#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>

#include <cstdio>


namespace WZMapEditor
{

Map3DViewWidget::Map3DViewWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent),
	m_moving(false),
	m_rotating(false),
	m_rotationX(0),
	m_rotationY(0),
	m_rotationZ(0),
	m_offsetX(0),
	m_offsetY(0),
	m_zoom(-20)
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

	connect(moveLeft, SIGNAL(triggered()), this, SLOT(doMapMoveLeft()));
	connect(moveRight, SIGNAL(triggered()), this, SLOT(doMapMoveRight()));
	connect(moveUp, SIGNAL(triggered()), this, SLOT(doMapMoveUp()));
	connect(moveDown, SIGNAL(triggered()), this, SLOT(doMapMoveDown()));

	setFocus();
}

void Map3DViewWidget::initializeGL()
{
	qglClearColor(QColor(0, 0, 32, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	//      glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//      glHint(GL_PERSPECTIVE_CORECTION_HINT, GL_NICEST);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Map3DViewWidget::resizeGL(int width, int height)
{
	//      int size = qMin(width, height);
	//      glViewport((width - size) / 2, (height - size) / 2, size, size);
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

	//      glEnable (GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glTranslatef(0.0f, 0.0f, m_zoom);
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
	int center_factor_x = (m_mapInformation->size().width()  * 1.0f) / 2;
	int center_factor_y = (m_mapInformation->size().height() * 1.0f) / 2;

	for (int i = 1; i <= m_mapInformation->size().width(); ++i)
	{
		for (int j = 1; j <= m_mapInformation->size().height(); ++j)
		{
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f , 0.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x, 1.0f * j - center_factor_y - 1.0f, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x, 1.0f * j - center_factor_y, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y,  0.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3f(1.0f , 0.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y - 1.0f, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f * i - center_factor_x, 1.0f * j - center_factor_y - 1.0f, 0.0f);
			glEnd();
		}
	}
}

void Map3DViewWidget::wheelEvent(QWheelEvent *event)
{
	m_zoom += event->delta() / 32;

	repaint();
}

void Map3DViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = true;
	}
}

void Map3DViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton)
	{
		QMenu menu;

		QAction *openAct = new QAction("not yet implemented", this);

		menu.addAction(openAct);
		menu.exec(mapToGlobal(event->pos()));
	}

	if (event->button() == Qt::LeftButton)
	{
		m_moving = false;
	}
}

void Map3DViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	static int last_move_x, last_move_y;

	if (m_moving == true)
	{
		m_rotationZ += -(last_move_x - event->pos().x());
		m_rotationX += -(last_move_y - event->pos().y());

		repaint();
	}

	last_move_x = event->pos().x();
	last_move_y = event->pos().y();

	emit cooridantesChanged(event->pos().x(), event->pos().y(), 0);
}

void Map3DViewWidget::resizeMap(int width, int height)
{
	if (m_mapInformation)
	{
		m_mapInformation->setSize(QSize(width, height));
	}

	repaint();
}

void Map3DViewWidget::doMapMoveLeft()
{
	m_offsetX += 1;

	repaint();
}

void Map3DViewWidget::doMapMoveRight()
{
	m_offsetX -= 1;

	repaint();
}

void Map3DViewWidget::doMapMoveUp()
{
	m_offsetY -= 1;

	repaint();
}

void Map3DViewWidget::doMapMoveDown()
{
	m_offsetY += 1;

	repaint();
}

void Map3DViewWidget::setMapInformation(MapInformation *data)
{
	m_mapInformation = data;

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

}
