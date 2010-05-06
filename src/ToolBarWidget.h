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

	void setObjectName(const QString &name);

public slots:
	void configure();
	void restoreDefaults();
	void load(QList<QAction*> actions);
	void setActions(const QStringList &actions);

protected:
	void contextMenuEvent(QContextMenuEvent *event);

private:
	QStringList m_actions;
	bool m_dirty;

signals:
	void configureRequested(ToolBarWidget *toolBar);
	void lockRequested(bool locked);
};

}

#endif // TOOLBARWIDGET_H
