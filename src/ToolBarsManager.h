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
	ToolBarsManager(QList<ToolBarWidget*> toolBars, ToolBarWidget *configureToolBar = NULL, QObject *parent = NULL);
	~ToolBarsManager();

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
	int m_currentToolBar;
	bool m_isCurrentModified;

signals:
	void toolBarChanged();
};

}

#endif
