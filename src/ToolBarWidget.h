#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMainWindow>

namespace WZMapEditor
{

class ToolBarWidget : public QToolBar
{
	Q_OBJECT

public:
	ToolBarWidget(const QString &identifier, QMainWindow *parent = NULL);

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

#endif
