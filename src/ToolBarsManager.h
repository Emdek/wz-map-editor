#ifndef TOOLBARSMANAGER_H
#define TOOLBARSMANAGER_H

#include <QtCore/QObject>

class QAbstractButton;

namespace Ui
{
	class ToolBarEditorDialog;
}

namespace WZMapEditor
{

class ToolBarWidget;

class ToolBarsManager : public QObject
{
	Q_OBJECT

public:
	ToolBarsManager(QList<ToolBarWidget*> widgets, const QString &configureToolBar = QString(), QObject *parent = NULL);
	~ToolBarsManager();

	void reloadToolbars();

protected slots:
	void loadToolBar(int index);
	void addToolBar();
	void removeToolBar();
	void renameToolBar();
	void availableActionsCurrentItemChanged(int index);
	void currentActionsCurrentItemChanged(int index);
	void removeItem();
	void addItem();
	void moveUpItem();
	void moveDownItem();
	void dialogButtonCliked(QAbstractButton *button);

private:
	Ui::ToolBarEditorDialog *m_ui;
	QList<ToolBarWidget*> m_widgets;
	int m_currentToolBar;
};

}

#endif
