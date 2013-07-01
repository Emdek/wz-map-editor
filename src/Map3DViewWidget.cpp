#include "Map3DViewWidget.h"
#include "Tileset.h"
#include "SettingsManager.h"
#include "Tileset.h"

#include <QtCore/QTime>
#include <QtGui/QMouseEvent>

#include <QDebug>

//#include <cstdio>
//#include <cmath>

namespace WZMapEditor
{

Map3DViewWidget::Map3DViewWidget(QWidget *parent) : QWidget(parent),
	m_map(NULL),
	m_moving(false),
	m_rotating(false),
	m_rotationX(0),
	m_rotationY(0),
	m_rotationZ(0),
	m_offsetX(0),
	m_offsetY(0),
	m_zoom(50),
	m_OGLinitialized(false)
{
	setMouseTracking(true);

	setFocus();
}

void Map3DViewWidget::initializeGL()
{
/*	m_OGLinitialized = true;

	qglClearColor(QColor(0, 0, 32, 0));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	setVBO();*/
}

void Map3DViewWidget::resizeGL(int width, int height)
{
	//	int size = qMin(width, height);
	//	glViewport((width - size) / 2, (height - size) / 2, size, size);
/*	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 1500.0);
	glMatrixMode(GL_MODELVIEW);*/
}

void Map3DViewWidget::paintGL()
{
	if (!m_map)
	{
		return;
	}

/*	QTime t;
	t.start();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);

	glTranslatef(0.0, 0.0, ((1.0 / m_zoom) * -2000));
	glRotatef(m_rotationX / 4.0, 1.0, 0.0, 0.0);
	glRotatef(m_rotationY / 4.0, 0.0, 1.0, 0.0);
	glRotatef(m_rotationZ / 4.0, 0.0, 0.0, 1.0);
	glTranslatef(m_offsetX, m_offsetY, 0.0);

	// VBO
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid *)offsetof(Vertex, pos));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex));

	glDrawArrays(GL_TRIANGLES, 0, m_vboData.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/

//	qDebug("Map3DViewWidget::paintGL(): redraw time: %d ms", t.elapsed());
}

void Map3DViewWidget::wheelEvent(QWheelEvent *event)
{
	setZoom(m_zoom + (event->delta() / 32));

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

		repaint();
	}

	if (m_moving == true)
	{
		m_offsetX -= (m_currentx - event->pos().x()) / (m_zoom / 1.65f);
		m_offsetY += (m_currenty - event->pos().y()) / (m_zoom / 1.65f);

		repaint();
	}

	m_currentx = event->pos().x();
	m_currenty = event->pos().y();

	emit cooridantesChanged(m_currentx, m_currenty, m_currentz);
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

		repaint();
	}
}

void Map3DViewWidget::setMap(Map *data)
{
/*	qDebug("Map3DWidget::setMap()");

	m_map = data;

	m_entities.clear();
	m_vboData.clear();

	const int tileSize = SettingManager::value("tileSize").toInt();

	for (int i = 1; i <= m_map->size().width(); ++i)
	{
		for (int j = 1; j <= m_map->size().height(); ++j)
		{
			GLint texid;

			// set objects
			Entity ent;
			ent.type = TerrainEntity;
			ent.x = i;
			ent.y = j;
			ent.selected = false;
			ent.hovered = false;

			// set verticles and textures for cache

			// this moves objects drawing position
			// simple hack to set center in really center of map - not on left-bottom edge
			int centerFactorX = (m_map->size().width()  * 1.0) / 2;
			int centerFactorY = (m_map->size().height() * 1.0) / 2;

			MapTile tile(m_map->tile((i - 1), (j - 1)));
			bool _old_tex = false;

			for (unsigned int x = 0; x < m_used_textures.size(); ++x)
			{
				if (m_used_textures[x].tileset == m_map->tileset() && m_used_textures[x].tiletexture == tile.texture)
				{
					ent.texid = m_used_textures[x].texid;
					_old_tex = true;

					break;
				}
			}

			if (_old_tex == false)
			{
				texid = bindTexture(Tileset::pixmap(m_map->tileset(), tile.texture, tileSize), GL_TEXTURE_2D, GL_RGBA, QGLContext::LinearFilteringBindOption);
				qDebug() << "bindTexture" << i << j << texid << tile.texture;
				used_texture t;
				t.tileset     = m_map->tileset();
				t.tiletexture = tile.texture;
				t.texid       = texid;
				m_used_textures.push_back(t);
			}

			if (texid == 0)
			{
				qDebug() << "error while loading texture" << m_map->tileset() << tile.texture;
			}

			float tile_left_top, tile_left_bottom, tile_right_top, tile_right_bottom;

			MapTile t_left_top(m_map->tile((i - 1), (j - 1)));
			tile_left_top = (t_left_top.height * 3.0f) / 255.0f;

			MapTile t_right_top(m_map->tile((i), (j - 1)));
			tile_right_top = (t_right_top.height * 3.0f) / 255.0f;

			if (j - 2 < 0)
			{
				tile_left_bottom = tile_left_top;
			}
			else
			{
				MapTile t_left_bottom(m_map->tile((i - 1), (j - 2)));
				tile_left_bottom = (t_left_bottom.height * 3.0f) / 255.0f;
			}

			if (j - 2 < 0)
			{
				tile_right_bottom = tile_right_top;
			}
			else
			{
				MapTile t_right_bottom(m_map->tile((i), (j - 2)));
				tile_right_bottom = (t_right_bottom.height * 3.0f) / 255.0f;
			}

			float posY = 1.0 * j - centerFactorY;
			float posX = 1.0 * i - centerFactorX;

			QPoint flip(((tile.flip & HorizontalFlip) ? -1.0 : 1.0), ((tile.flip & VerticalFlip) ? 1.0 : -1.0));
			float rotation((GLfloat)(tile.rotation + 90));

			Vertex v1 = {{posX,       posY - 1.0, tile_right_bottom}, {0, 0}};
			Vertex v2 = {{posX,       posY,       tile_right_top   }, {1, 0}};
			Vertex v3 = {{posX - 1.0, posY - 1.0, tile_left_bottom }, {0, 1}};
			Vertex v4 = {{posX - 1.0, posY,       tile_left_top    }, {1, 1}};

			m_vboData.push_back(v1);
			m_vboData.push_back(v2);
			m_vboData.push_back(v3);

			m_vboData.push_back(v4);
			m_vboData.push_back(v3);
			m_vboData.push_back(v2);

			ent.flip = flip;
			ent.rotation = rotation;

			m_entities.push_back(ent);
		}
	}

	setVBO();

	repaint();

	connect(m_map, SIGNAL(changed(QRect)), this, SLOT(repaint()));*/
}

Map* Map3DViewWidget::mapInformation()
{
	return m_map;
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

void Map3DViewWidget::setVBO()
{
/*	if (m_OGLinitialized)
	{
		GLuint vboId = 0;
		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vboData.size(), &m_vboData[0], GL_STATIC_DRAW);

		glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	}*/
}

void Map3DViewWidget::_glSelect(int x, int y)
{
/*	return;
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
	for (int i = 0; i < hits; i++)
	{
		int s = (GLubyte)buff[(i * 4) + 3];

		m_entities[s].hovered = true;

		qDebug() << QString("[hit %1] %2:\t%3 x %4").arg(i).arg(s).arg(m_entities[s].x).arg(m_entities[s].y);
	}

	//_listHits(hits,buff);

	glMatrixMode(GL_MODELVIEW);*/
}

/*void Map3DViewWidget::_listHits(GLint hits, GLuint *names)
{

		For each hit in the buffer are allocated 4 bytes:
		1. Number of hits selected (always one,
									beacuse when we draw each object
									we use glLoadName, so we replace the
									prevous name in the stack)
		2. Min Z
		3. Max Z
		4. Name of the hit (glLoadName)


	qDebug() << QString("%1 hits:").arg(hits);

	for (int i = 0; i < hits; i++)
	{
		qDebug() << "Number:" << (GLubyte)names[i * 4];
		qDebug() << "Min Z:" << (GLubyte)names[(i * 4) + 1];
		qDebug() << "Max Z:" << (GLubyte)names[(i * 4) + 2];
		qDebug() << "Name on stack:" << (GLubyte)names[(i * 4) + 3];
	}

	qDebug() << "";
}*/

}
