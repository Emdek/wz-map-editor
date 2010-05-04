#include "MapEditorWidget.h"

#include <QTimer>
#include <QtGui/QMenu>
#include <QMouseEvent>
#include <cstdio>

namespace WZMapEditor
{

MapEditorWidget::MapEditorWidget(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(repaint()));
	timer->start(10);

	this->m_moving = false;
	this->rot_x = 0;
	this->rot_y = 0;
	this->rot_z = 0;
	this->zoom_value = -100.0;

	this->setMouseTracking(true);
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

	glTranslatef(0.0, 0.0, this->zoom_value);
	glRotatef(rot_x / 4.0f, 1.0, 0.0, 0.0);
	glRotatef(rot_y / 4.0f, 0.0, 1.0, 0.0);
	glRotatef(rot_z / 4.0f, 0.0, 0.0, 1.0);

	glBegin(GL_TRIANGLES);
	glVertex3f( 10.0f, -10.0f, 0.0f);
	glVertex3f( 10.0f,  10.0f, 0.0f);
	glVertex3f(-10.0f,  10.0f, 0.0f);
	glEnd();

	QCursor c = QCursor(this->cursor());
	QPoint p = this->mapFromGlobal(c.pos());
	emit this->changeCoordsX(p.x());
	emit this->changeCoordsY(p.y());
}

void MapEditorWidget::wheelEvent (QWheelEvent *event)
{
	this->zoom_value += event->delta() / 8;
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

		QAction *openAct = new QAction("Open...", this);

		menu.addSeparator();
		menu.addAction(openAct);
		menu.addAction(openAct);
		menu.addAction(openAct);

		menu.addSeparator();
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
	}
	last_move_x = event->pos().x();
	last_move_y = event->pos().y();
}


}
