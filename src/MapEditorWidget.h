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
	int rot_x;
	int rot_y;
	int rot_z;
	float zoom_value;
};

}

#endif // QMAPEDITOR_H
