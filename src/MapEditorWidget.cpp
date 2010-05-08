#include "MapEditorWidget.h"

#include <QtGui/QMenu>
#include <QMouseEvent>

#include <cstdio>

namespace WZMapEditor
{

MapEditorWidget::MapEditorWidget(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	this->m_moving = false;
	this->rot_x = 0;
	this->rot_y = 0;
	this->rot_z = 0;
	this->zoom_value = -20.0;

	this->setMouseTracking(true);

	this->map_width = 7;
	this->map_height = 5;

	this->map_move_x = 0;
	this->map_move_y = 0;

	QAction *moveLeft  = new QAction(this);
	QAction *moveRight = new QAction(this);
	QAction *moveUp    = new QAction(this);
	QAction *moveDown  = new QAction(this);
	moveLeft-> setShortcut(Qt::Key_Left);
	moveRight->setShortcut(Qt::Key_Right);
	moveUp->   setShortcut(Qt::Key_Up);
	moveDown-> setShortcut(Qt::Key_Down);

	this->addAction(moveLeft);
	this->addAction(moveRight);
	this->addAction(moveUp);
	this->addAction(moveDown);

	connect(moveLeft,  SIGNAL(triggered()), this, SLOT(doMapMoveLeft()));
	connect(moveRight, SIGNAL(triggered()), this, SLOT(doMapMoveRight()));
	connect(moveUp,    SIGNAL(triggered()), this, SLOT(doMapMoveUp()));
	connect(moveDown,  SIGNAL(triggered()), this, SLOT(doMapMoveDown()));

	this->setFocus();
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

	glTranslatef(0.0f, 0.0f, this->zoom_value);
	glRotatef(rot_x / 4.0f, 1.0, 0.0, 0.0);
	glRotatef(rot_y / 4.0f, 0.0, 1.0, 0.0);
	glRotatef(rot_z / 4.0f, 0.0, 0.0, 1.0);
	glTranslatef(this->map_move_x * 0.2f, this->map_move_y * 0.2f, 0.0f);

	// this moves objects drawing position
	// simple hack to set center in really center of map - not on left-bottom edge
	int center_factor_x = (this->map_width  * 1.0f) / 2;
	int center_factor_y = (this->map_height * 1.0f) / 2;

	int i, j;
	for (i = 1; i <= this->map_width; i++)
	{
		for (j = 1; j <= this->map_height; j++)
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
	this->zoom_value += event->delta() / 32;
	this->repaint();
}

void MapEditorWidget::mousePressEvent (QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		this->m_moving = true;
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
		this->m_moving = false;
	}
}

void MapEditorWidget::mouseMoveEvent (QMouseEvent *event)
{
	static int last_move_x, last_move_y;
	if (m_moving == true)
	{
		this->rot_z += -(last_move_x - event->pos().x());
		this->rot_x += -(last_move_y - event->pos().y());

		this->repaint();
	}
	last_move_x = event->pos().x();
	last_move_y = event->pos().y();

	emit this->changeCoordsX(event->pos().x());
	emit this->changeCoordsY(event->pos().y());
}

void MapEditorWidget::resizeMap (unsigned int width, unsigned int height)
{
	this->map_width  = width;
	this->map_height = height;
	this->repaint();
}

void MapEditorWidget::doMapMoveLeft()
{
	this->map_move_x += 1;
	this->repaint();
}

void MapEditorWidget::doMapMoveRight()
{
	this->map_move_x -= 1;
	this->repaint();
}

void MapEditorWidget::doMapMoveUp()
{
	this->map_move_y -= 1;
	this->repaint();
}

void MapEditorWidget::doMapMoveDown()
{
	this->map_move_y += 1;
	this->repaint();
}

}
