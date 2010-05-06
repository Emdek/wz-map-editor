#include "ShortcutEditorDelegate.h"
#include "ShortcutManager.h"
#include "ShortcutEditorWidget.h"

namespace WZMapEditor
{

ShortcutEditorDelegate::ShortcutEditorDelegate(ShortcutManager *parent) : QItemDelegate(parent),
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
	editorWidget->setSequence(index.model()->data(index, Qt::EditRole).toString());
}

void ShortcutEditorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	ShortcutEditorWidget *editorWidget = static_cast<ShortcutEditorWidget*>(editor);

	QKeySequence sequence = editorWidget->keySequence();

	if (m_manager->checkSequence(sequence, index.row()))
	{
		model->setData(index, sequence.toString(QKeySequence::NativeText), Qt::EditRole);
	}
}

void ShortcutEditorDelegate::restore()
{
	ShortcutEditorWidget *editor = static_cast<ShortcutEditorWidget*>(sender());
	editor->setSequence(m_manager->restoreDefaultShortcut(editor->index()));
}

QWidget* ShortcutEditorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	Q_UNUSED(option)

	ShortcutEditorWidget *editor = new ShortcutEditorWidget(index.model()->data(index, Qt::EditRole).toString(), index.row(), parent);

	connect(editor, SIGNAL(restore()), this, SLOT(restore()));

	return editor;
}

}
