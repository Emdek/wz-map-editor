#ifndef QMAPEDITOR_H
#define QMAPEDITOR_H

#include <QtOpenGL/QGLWidget>

namespace WZMapEditor
{

class MapEditorWidget : public QGLWidget
{
public:
	MapEditorWidget(QWidget *parent = 0);
	~MapEditorWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
};

}

#endif // QMAPEDITOR_H
