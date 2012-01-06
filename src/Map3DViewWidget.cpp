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

	setFocus();
}

void Map3DViewWidget::initializeGL()
{
	qglClearColor(QColor(0, 0, 32, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

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
	glMatrixMode(GL_MODELVIEW);
}

void Map3DViewWidget::paintGL()
{
	qDebug("paintGL();");
	int s;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glTranslatef(0.0, 0.0, ((1.0 / m_zoom) * -2000));
	glRotatef(m_rotationX / 4.0, 1.0, 0.0, 0.0);
	glRotatef(m_rotationY / 4.0, 0.0, 1.0, 0.0);
	glRotatef(m_rotationZ / 4.0, 0.0, 0.0, 1.0);
	glTranslatef(m_offsetX, m_offsetY, 0.0);

	if (!m_mapInformation)
	{
		return;
	}

	// this moves objects drawing position
	// simple hack to set center in really center of map - not on left-bottom edge
	int centerFactorX = (m_mapInformation->size().width()  * 1.0) / 2;
	int centerFactorY = (m_mapInformation->size().height() * 1.0) / 2;

	int counter = 0;
	const int tileSize = SettingManager::value("tileSize").toInt();

	for (int i = 1; i <= m_mapInformation->size().width(); ++i)
	{
		for (int j = 1; j <= m_mapInformation->size().height(); ++j)
		{
			glLoadName(counter);
			counter++;

			glMatrixMode(GL_TEXTURE);

			QPointF coordinates(((1.0 * i) - centerFactorX), ((1.0 * j) - centerFactorY));
			MapTile tile(m_mapInformation->tile((i - 1), (j - 1)));

			glPushMatrix();

			glScalef(((tile.flip & FlipTypeHorizontal) ? -1.0 : 1.0), ((tile.flip & FlipTypeVertical) ? 1.0 : -1.0), 0.0);
			glRotatef((GLfloat)(tile.rotation + 90) * 1.0, 0.0, 0.0, 1.0);

			bindTexture(Tileset::pixmap(m_mapInformation->tileset(), tile.texture, tileSize), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			float tile_left_top, tile_left_bottom, tile_right_top, tile_right_bottom;

			MapTile t_left_top(m_mapInformation->tile((i - 1), (j - 1)));
			tile_left_top = (t_left_top.height * 3.0f) / 255.0f;

			MapTile t_right_top(m_mapInformation->tile((i), (j - 1)));
			tile_right_top = (t_right_top.height * 3.0f) / 255.0f;

			if (j - 2 < 0)
			{
				tile_left_bottom = tile_left_top;
			}
			else
			{
				MapTile t_left_bottom(m_mapInformation->tile((i - 1), (j - 2)));
				tile_left_bottom = (t_left_bottom.height * 3.0f) / 255.0f;
			}

			if (j - 2 < 0)
			{
				tile_right_bottom = tile_right_top;
			}
			else
			{
				MapTile t_right_bottom(m_mapInformation->tile((i), (j - 2)));
				tile_right_bottom = (t_right_bottom.height * 3.0f) / 255.0f;
			}

			float posY = 1.0 * j - centerFactorY;
			float posX = 1.0 * i - centerFactorX;

			glColor3f(1.0f, 1.0f, 1.0f);
			// selection code
			for (s = 0; s < m_selected.size(); s++)
			{
				if (m_objects[m_selected[s]].x == i && m_objects[m_selected[s]].y == j)
				{
					glColor3f(1.0f, 0.6f, 0.5f);
				}
			}
			// selection code ends here

			glBegin(GL_TRIANGLES);
			{
				glTexCoord2f(0, 0);
				glVertex3f(posX, posY - 1.0, tile_right_bottom);

				glTexCoord2f(1, 0);
				glVertex3f(posX, posY, tile_right_top);

				glTexCoord2f(1, 1);
				glVertex3f(posX - 1.0, posY, tile_left_top);

				glTexCoord2f(1, 1);
				glVertex3f(posX - 1.0, posY, tile_left_top);

				glTexCoord2f(0, 1);
				glVertex3f(posX - 1.0, posY - 1.0, tile_left_bottom);

				glTexCoord2f(0, 0);
				glVertex3f(posX, posY - 1.0, tile_right_bottom);
			}
			glEnd();

			glPopMatrix();
		}
	}
	glMatrixMode(GL_MODELVIEW);

	m_selected.clear();
}

void Map3DViewWidget::wheelEvent(QWheelEvent *event)
{
	setZoom(m_zoom + (event->delta() / 32));

	_glSelect(m_currentx, height() - m_currenty);
	repaint();

	event->accept();
}

void Map3DViewWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = true;
	}
	else if (event->button() == Qt::RightButton)
	{
		m_rotating = true;
	}
	else
	{
		event->ignore();
	}
	event->accept();
}

void Map3DViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_moving = false;
	}
	else if (event->button() == Qt::RightButton)
	{
		m_rotating = false;
	}
	else
	{
		event->ignore();
	}
	event->accept();
}

void Map3DViewWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_rotating == true)
	{
		m_rotationZ -= (m_currentx - event->pos().x());
		m_rotationX -= (m_currenty - event->pos().y());
	}
	if (m_moving == true)
	{
		m_offsetX -= (m_currentx - event->pos().x()) / (m_zoom / 1.65f);
		m_offsetY += (m_currenty - event->pos().y()) / (m_zoom / 1.65f);
	}
	m_currentx = event->pos().x();
	m_currenty = event->pos().y();

	_glSelect(event->pos().x(), height() - event->pos().y());
	repaint();

	emit cooridantesChanged(m_currentx, m_currenty, m_currentz);
}

void Map3DViewWidget::contextMenuEvent(QContextMenuEvent *event)
{
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
		m_currentz = (500 - m_zoom);

		emit zoomChanged(zoom);
		emit cooridantesChanged(m_currentx, m_currenty, m_currentz);
	}
}

void Map3DViewWidget::setMapInformation(MapInformation *data)
{
	m_mapInformation = data;

	// selection code
	m_objects.clear();
	for (int i = 1; i <= m_mapInformation->size().width(); ++i)
	{
		for (int j = 1; j <= m_mapInformation->size().height(); ++j)
		{
			objects current_object;
			current_object.type     = TYPE_TERRAIN;
			current_object.x        = i;
			current_object.y        = j;
			m_objects.push_back(current_object);
		}
	}
	//printf("COUNT:  %i\n\n", m_objects.size());
	// selection code ends here

	repaint();

	connect(m_mapInformation, SIGNAL(changed()), this, SLOT(repaint()));
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
	int id;
	return QSize(320, 240);
}

int Map3DViewWidget::zoom()
{
	return m_zoom;
}

void Map3DViewWidget::_glSelect(int x, int y)
{
	GLuint buff[64] = {0};
	GLint hits, view[4];

	glSelectBuffer(64, buff);
	glGetIntegerv(GL_VIEWPORT, view);
	glRenderMode(GL_SELECT);

	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x, y, 0.0001, 0.0001, view);
	gluPerspective(45.0, (GLfloat)view[2] / (GLfloat)view[3], 0.1, 1500.0);

	glMatrixMode(GL_MODELVIEW);

	repaint();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	hits = glRenderMode(GL_RENDER);

	// select ALL things where mouse is
	int i;
	for (i = 0; i < hits; i++)
	{
		int s = (GLubyte)buff[i * 4 + 3];
		m_selected.push_back(s);
		printf("[hit %i] %i:\t%i x %i\n", i, s, m_objects[m_selected.size()-1].x, m_objects[m_selected.size()-1].y);
	}
	printf("\n");
	//_listHits(hits,buff);

	glMatrixMode(GL_MODELVIEW);
}

void Map3DViewWidget::_listHits(GLint hits, GLuint *names)
{
	int i;

	/*
		For each hit in the buffer are allocated 4 bytes:
		1. Number of hits selected (always one,
									beacuse when we draw each object
									we use glLoadName, so we replace the
									prevous name in the stack)
		2. Min Z
		3. Max Z
		4. Name of the hit (glLoadName)
	*/

	printf("%d hits:\n", hits);

	for (i = 0; i < hits; i++)
	{
		printf(	"Number: %d\n"
				"Min Z: %d\n"
				"Max Z: %d\n"
				"Name on stack: %d\n",
				(GLubyte)names[i * 4],
				(GLubyte)names[i * 4 + 1],
				(GLubyte)names[i * 4 + 2],
				(GLubyte)names[i * 4 + 3]
				);
	}
	printf("\n");
}

}
