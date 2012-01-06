#ifndef MAP3DVIEWWIDGET_H
#define MAP3DVIEWWIDGET_H

#include <QtCore/QHash>
#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>

#include <vector>

#include "Tileset.h"

namespace WZMapEditor
{

class Map;

struct Vertex
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

enum EntityType {
	TypeTerrain,
	TypeStructure
};

struct Entity
{
	//Object(Vertex v[4], GLuint t, QPoint f, float r);
	EntityType type;

	// position on wz map
	int        x;
	int        y;

	bool       hovered;
	bool       selected;

	std::vector<Vertex> vertex; // object verticles

	GLuint texid;
	QPoint flip;
	float  rotation;
};

class Map3DViewWidget : public QGLWidget
{
	Q_OBJECT

public:
	Map3DViewWidget(QWidget *parent = NULL);

	void setMap(Map *data);

	void _glSelect(int x, int y);
	void _listHits(GLint hits, GLuint *names);

	Map* mapInformation();
	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	int zoom();

public slots:
	void setZoom(qreal zoom);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

private:
	Map *m_map;
	bool m_moving;
	bool m_rotating;
	int m_rotationX;
	int m_rotationY;
	int m_rotationZ;
	int m_currentx;
	int m_currenty;
	int m_currentz;
	float m_offsetX;
	float m_offsetY;
	int m_zoom;

	std::vector<Entity> m_entities;
//	std::vector<int> m_selected;

	struct used_texture
	{
		TilesetType tileset;
		int         tiletexture;
		int         texid;
	};
	std::vector<used_texture> m_used_textures;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int zoom);
};

}

#endif // MAP3DVIEWWIDGET_H
