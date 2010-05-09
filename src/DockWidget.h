#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QtGui/QDockWidget>

namespace WZMapEditor
{

class DockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DockWidget(const QString &title, QWidget *parent = NULL);

protected:
	void showEvent(QShowEvent *event);
};

}

#endif // DOCKWIDGET_H
