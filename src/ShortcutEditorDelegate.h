#ifndef SHORTCUTEDITORDELEGATE_H
#define SHORTCUTEDITORDELEGATE_H

#include <QtWidgets/QItemDelegate>

namespace WZMapEditor
{

class ShortcutManager;

class ShortcutEditorDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ShortcutEditorDelegate(ShortcutManager *parent = NULL);

	void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget *editor, const QModelIndex &index) const;
	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public slots:
	void restore();

private:
	ShortcutManager *m_manager;
	int m_currentIndex;
};

}

#endif
