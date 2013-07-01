#include "ShortcutEditorDelegate.h"
#include "ActionsManager.h"
#include "ShortcutsManager.h"
#include "ShortcutEditorWidget.h"

namespace WZMapEditor
{

ShortcutEditorDelegate::ShortcutEditorDelegate(ShortcutsManager *parent) : QItemDelegate(parent),
	m_manager(parent),
	m_currentIndex(-1)
{
}

void ShortcutEditorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(index)

	editor->setGeometry(option.rect);
}

void ShortcutEditorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	ShortcutEditorWidget *editorWidget = static_cast<ShortcutEditorWidget*>(editor);
	editorWidget->setShortcut(index.model()->data(index, Qt::EditRole).toString());
}

void ShortcutEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ShortcutEditorWidget *editorWidget = static_cast<ShortcutEditorWidget*>(editor);
	QKeySequence shortcut = editorWidget->getShortcut();

	if (m_manager->checkShortcut(shortcut, index.row()))
	{
		model->setData(index, shortcut.toString(QKeySequence::NativeText), Qt::EditRole);
	}
}

void ShortcutEditorDelegate::restore()
{
	ShortcutEditorWidget *editor = static_cast<ShortcutEditorWidget*>(sender());
	editor->setShortcut(ActionsManager::getDefaultShortcut(editor->getAction()));
}

QWidget* ShortcutEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)

	ShortcutEditorWidget *editor = new ShortcutEditorWidget(index.data(Qt::UserRole).toString(), index.data(Qt::EditRole).toString(), parent);

	connect(editor, SIGNAL(restore()), this, SLOT(restore()));

	return editor;
}

}
