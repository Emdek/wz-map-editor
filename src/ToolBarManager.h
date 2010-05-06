#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QtCore/QHash>
#include <QtCore/QObject>

#include <QtGui/QToolBar>
#include <QtGui/QAbstractButton>

namespace Ui
{
	class ToolBarEditorDialog;
}

namespace WZMapEditor
{

class ToolBarWidget;

class ToolBarManager : public QObject
{
	Q_OBJECT

public:
	ToolBarManager(QList<QAction*> actions, QList<ToolBarWidget*> toolBars, ToolBarWidget *configureToolBar = NULL, QObject *parent = NULL);
	~ToolBarManager();

public slots:
	void loadToolBar(int index);
	void availableActionsCurrentItemChanged(int index);
	void currentActionsCurrentItemChanged(int index);
	void removeItem();
	void addItem();
	void moveUpItem();
	void moveDownItem();
	void dialogButtonCliked(QAbstractButton *button);
	void saveToolBar();
	void restoreToolBar();
	void setModified(bool modified = true);

private:
	Ui::ToolBarEditorDialog *m_managerUi;
	QList<ToolBarWidget*> m_toolBars;
	QList<QAction*> m_availableActions;
	int m_currentToolBar;
	bool m_isCurrentModified;

signals:
	void toolBarChanged();

};

}

#endif // TOOLBARMANAGER_H
