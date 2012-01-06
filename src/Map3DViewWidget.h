#ifndef MAP3DVIEWWIDGET_H
#define MAP3DVIEWWIDGET_H

#include <QtCore/QHash>
#include <QtOpenGL/QGLWidget>
#include <GL/glu.h>

#include <vector>

namespace WZMapEditor
{

class Map;

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

	enum object_type {
		TYPE_TERRAIN,
		TYPE_STRUCTURE
	};

	struct objects {
		object_type type;
		int         x;
		int         y;
		bool        selected;
	};
	std::vector<objects> m_objects;
	std::vector<int> m_selected;

signals:
	void cooridantesChanged(int x, int y, int z);
	void zoomChanged(int zoom);
};

}

#endif // MAP3DVIEWWIDGET_H
