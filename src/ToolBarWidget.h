#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QtGui/QToolBar>
#include <QtGui/QMainWindow>
#include <QtGui/QContextMenuEvent>

namespace WZMapEditor
{

class ToolBarWidget : public QToolBar
{
	Q_OBJECT

public:
	ToolBarWidget(QMainWindow *parent = NULL);

public slots:
	void reload();

protected:
	void showEvent(QShowEvent *event);
	void hideEvent(QHideEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);

signals:
	void visibilityChanged(bool visible);
};

}

#endif // TOOLBARWIDGET_H
