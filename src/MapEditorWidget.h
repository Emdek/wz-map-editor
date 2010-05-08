#ifndef QMAPEDITOR_H
#define QMAPEDITOR_H

#include <QtOpenGL/QGLWidget>

namespace WZMapEditor
{

class MapEditorWidget : public QGLWidget
{
	Q_OBJECT
public:
	MapEditorWidget(QWidget *parent = 0);
	~MapEditorWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

signals:
	void changeCoordsX (int coords);
	void changeCoordsY (int coords);
	void changeCoordsZ (int coords);

public slots:
	void doMapMoveLeft  ();
	void doMapMoveRight ();
	void doMapMoveUp    ();
	void doMapMoveDown  ();
	void resizeMap (unsigned int width, unsigned int height);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void wheelEvent        (QWheelEvent *event);
	void mousePressEvent   (QMouseEvent *event);
	void mouseReleaseEvent (QMouseEvent *event);
	void mouseMoveEvent    (QMouseEvent *event);

private:
	bool m_moving;
	bool m_rotating;

	int rot_x;
	int rot_y;
	int rot_z;

	int map_move_x;
	int map_move_y;
	float zoom_value;

	unsigned int map_width;
	unsigned int map_height;
};

}

#endif // QMAPEDITOR_H
