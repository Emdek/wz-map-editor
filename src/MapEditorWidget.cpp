#include "MapEditorWidget.h"

#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>

#include <cstdio>

namespace WZMapEditor
{

MapEditorWidget::MapEditorWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	m_moving = false;
	rot_x = 0;
	rot_y = 0;
	rot_z = 0;
	zoom_value = -20.0;

	setMouseTracking(true);

	map_width = 7;
	map_height = 5;

	map_move_x = 0;
	map_move_y = 0;

	QAction *moveLeft = new QAction(this);
	QAction *moveRight = new QAction(this);
	QAction *moveUp = new QAction(this);
	QAction *moveDown = new QAction(this);

	moveLeft->setShortcut(Qt::Key_Left);
	moveRight->setShortcut(Qt::Key_Right);
	moveUp-> setShortcut(Qt::Key_Up);
	moveDown-> setShortcut(Qt::Key_Down);

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

MapEditorWidget::~MapEditorWidget()
{
}


QSize MapEditorWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize MapEditorWidget::sizeHint() const
{
	return QSize(320, 240);
}

void MapEditorWidget::initializeGL()
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

void MapEditorWidget::resizeGL(int width, int height)
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

void MapEditorWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//      glEnable (GL_LIGHTING);
	glEnable (GL_TEXTURE_2D);

	glTranslatef(0.0f, 0.0f, zoom_value);
	glRotatef(rot_x / 4.0f, 1.0, 0.0, 0.0);
	glRotatef(rot_y / 4.0f, 0.0, 1.0, 0.0);
	glRotatef(rot_z / 4.0f, 0.0, 0.0, 1.0);
	glTranslatef(this->map_move_x * 0.2f, map_move_y * 0.2f, 0.0f);

	// this moves objects drawing position
	// simple hack to set center in really center of map - not on left-bottom edge
	int center_factor_x = (map_width  * 1.0f) / 2;
	int center_factor_y = (map_height * 1.0f) / 2;

	for (int i = 1; i <= map_width; ++i)
	{
		for (int j = 1; j <= map_height; ++j)
		{
			glBegin(GL_TRIANGLES);
			glColor3f(1.0f , 0.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x,        1.0f * j - center_factor_y - 1.0f, 0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x,        1.0f * j - center_factor_y,        0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y,        0.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3f(1.0f , 0.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y,        0.0f);

			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(1.0f * i - center_factor_x - 1.0f, 1.0f * j - center_factor_y - 1.0f, 0.0f);

			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(1.0f * i - center_factor_x,        1.0f * j - center_factor_y - 1.0f, 0.0f);
			glEnd();
		}
	}

}

void MapEditorWidget::wheelEvent (QWheelEvent *event)
{
	zoom_value += event->delta() / 32;

	repaint();
}

void MapEditorWidget::mousePressEvent (QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = true;
	}
}

void MapEditorWidget::mouseReleaseEvent (QMouseEvent *event)
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

void MapEditorWidget::mouseMoveEvent (QMouseEvent *event)
{
	static int last_move_x, last_move_y;

	if (m_moving == true)
	{
		rot_z += -(last_move_x - event->pos().x());
		rot_x += -(last_move_y - event->pos().y());

		repaint();
	}

	last_move_x = event->pos().x();
	last_move_y = event->pos().y();

	emit cooridantesChanged(event->pos().x(), event->pos().y(), 0);
}

void MapEditorWidget::resizeMap (unsigned int width, unsigned int height)
{
	map_width  = width;
	map_height = height;

	repaint();
}

void MapEditorWidget::doMapMoveLeft()
{
	map_move_x += 1;

	repaint();
}

void MapEditorWidget::doMapMoveRight()
{
	map_move_x -= 1;

	repaint();
}

void MapEditorWidget::doMapMoveUp()
{
	map_move_y -= 1;

	repaint();
}

void MapEditorWidget::doMapMoveDown()
{
	map_move_y += 1;

	repaint();
}

}
