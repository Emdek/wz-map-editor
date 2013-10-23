#include "ShortcutEditorWidget.h"

#include <QtGui/QKeyEvent>
#include <QtWidgets/QHBoxLayout>

namespace WZMapEditor
{

ShortcutEditorWidget::ShortcutEditorWidget(const QString &action, const QString &shortcut, QWidget *parent) : QWidget(parent),
	m_action(action),
	m_sequenceEdit(new QKeySequenceEdit(this)),
	m_clearButton(new QToolButton(this)),
	m_restoreButton(new QToolButton(this))
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_sequenceEdit);
	layout->addWidget(m_clearButton);
	layout->addWidget(m_restoreButton);

	m_sequenceEdit->setToolTip(tr("Shortcut"));
	m_sequenceEdit->setKeySequence(shortcut);

	m_clearButton->setToolTip(tr("Clear"));
	m_clearButton->setIcon(QIcon(":/icons/edit-clear-locationbar-rtl.png"));
	m_clearButton->setCheckable(false);
	m_clearButton->setEnabled(!shortcut.isEmpty());

	m_restoreButton->setToolTip(tr("Restore Default"));
	m_restoreButton->setIcon(QIcon(":/icons/edit-undo.png"));

	setLayout(layout);

	setFocusPolicy(Qt::StrongFocus);

	connect(m_sequenceEdit, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(shortcutChanged()));
	connect(m_clearButton, SIGNAL(clicked()), m_sequenceEdit, SLOT(clear()));
	connect(m_restoreButton, SIGNAL(clicked()), this, SIGNAL(restore()));
}

void ShortcutEditorWidget::shortcutChanged()
{
	m_clearButton->setEnabled(!m_sequenceEdit->keySequence().isEmpty());
}

void ShortcutEditorWidget::setShortcut(const QKeySequence &shortcut)
{
	m_sequenceEdit->setKeySequence(shortcut);

	shortcutChanged();
}

QString ShortcutEditorWidget::getAction() const
{
	return m_action;
}

QKeySequence ShortcutEditorWidget::getShortcut() const
{
	return m_sequenceEdit->keySequence();
}

}
