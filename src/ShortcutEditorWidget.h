#ifndef SHORTCUTEDITORWIDGET_H
#define SHORTCUTEDITORWIDGET_H

#include <QtGui/QKeySequence>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QKeySequenceEdit>

namespace WZMapEditor
{

class ShortcutEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ShortcutEditorWidget(const QString &action, const QString &shortcut, QWidget *parent = NULL);

	QString getAction() const;
	QKeySequence getShortcut() const;

public slots:
	void setShortcut(const QKeySequence &shortcut);

protected slots:
	void shortcutChanged();

private:
	QKeySequenceEdit *m_sequenceEdit;
	QToolButton *m_clearButton;
	QToolButton *m_restoreButton;
	QString m_action;

signals:
	void restore();
};

}

#endif
