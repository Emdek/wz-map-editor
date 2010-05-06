#include "ShortcutEditorWidget.h"

#include <QtGui/QKeyEvent>
#include <QtGui/QHBoxLayout>

namespace WZMapEditor
{

ShortcutEditorWidget::ShortcutEditorWidget(const QString &keySequence, int index, QWidget *parent) : QWidget(parent),
	m_keySequence(keySequence),
	m_lineEdit(new QLineEdit(this)),
	m_clearButton(new QToolButton(this)),
	m_restoreButton(new QToolButton(this)),
	m_index(index),
	m_isRecording(false)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_lineEdit);
	layout->addWidget(m_clearButton);
	layout->addWidget(m_restoreButton);

	m_lineEdit->setToolTip(tr("Shortcut"));
	m_lineEdit->setReadOnly(true);
	m_lineEdit->installEventFilter(this);
	m_lineEdit->setFocusPolicy(Qt::StrongFocus);

	m_clearButton->setToolTip(tr("Clear"));
	m_clearButton->setIcon(QIcon(":/icons/edit-clear-locationbar-rtl.png"));
	m_clearButton->setCheckable(false);
	m_clearButton->setEnabled(!m_keySequence.isEmpty());

	m_restoreButton->setToolTip(tr("Restore Default"));
	m_restoreButton->setIcon(QIcon(":/icons/edit-undo.png"));

	setLayout(layout);

	setFocusPolicy(Qt::StrongFocus);

	updateDisplay();

	connect(m_clearButton, SIGNAL(clicked()), this, SLOT(clear()));
	connect(m_restoreButton, SIGNAL(clicked()), this, SIGNAL(restore()));
}

void ShortcutEditorWidget::updateDisplay()
{
	m_lineEdit->setText(m_keySequence.toString(QKeySequence::NativeText));
}

void ShortcutEditorWidget::clear()
{
	m_keySequence = QKeySequence();

	m_clearButton->setEnabled(false);

	updateDisplay();
}

void ShortcutEditorWidget::setSequence(const QString &keySequence)
{
	m_keySequence = QKeySequence(keySequence);

	m_clearButton->setEnabled(!m_keySequence.isEmpty());

	updateDisplay();
}

QKeySequence ShortcutEditorWidget::keySequence() const
{
	return m_keySequence;
}

int ShortcutEditorWidget::index()
{
	return m_index;
}

bool ShortcutEditorWidget::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::ContextMenu)
	{
		m_lineEdit->setFocus();

		return true;
	}
	else if (event->type() == QEvent::FocusOut)
	{
		m_isRecording = false;

		updateDisplay();
	}
	else if (event->type() == QEvent::KeyPress)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		int key = keyEvent->key();

		if (key == -1 || keyEvent->isAutoRepeat())
		{
			return true;
		}

		if (!m_isRecording)
		{
			m_recordedSequence = QString();

			m_isRecording = true;
		}

		switch (key)
		{
			case Qt::Key_Control:
				key = Qt::CTRL;
			break;
			case Qt::Key_Shift:
				key = Qt::SHIFT;
			break;
			case Qt::Key_Alt:
				key = Qt::ALT;
			break;
			case Qt::Key_Meta:
				key = Qt::META;
			break;
			default:
			break;
		}

		if (m_isRecording)
		{
			if (!m_recordedSequence.isEmpty() && !m_recordedSequence.endsWith("+"))
			{
				m_recordedSequence.append("+");
			}

			m_recordedSequence.append(QKeySequence(key).toString());
			m_keySequence = QKeySequence(m_recordedSequence);

			updateDisplay();
		}
	}

	return QObject::eventFilter(object, event);
}

}
