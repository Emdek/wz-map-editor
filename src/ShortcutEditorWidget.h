#ifndef SHORTCUTEDITORWIDGET_H
#define SHORTCUTEDITORWIDGET_H

#include <QtGui/QKeySequence>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>

namespace WZMapEditor
{

class ShortcutEditorWidget : public QWidget
{
	Q_OBJECT

public:
	ShortcutEditorWidget(const QString &action, const QString &shortcut, QWidget *parent = NULL);

	void updateDisplay();
	QString getAction() const;
	QKeySequence getShortcut() const;
	bool eventFilter(QObject *object, QEvent *event);

public slots:
	void clear();
	void setShortcut(const QKeySequence &shortcut);

private:
	QString m_action;
	QString m_recording;
	QKeySequence m_shortcut;
	QLineEdit *m_lineEdit;
	QToolButton *m_clearButton;
	QToolButton *m_restoreButton;
	bool m_isRecording;

signals:
	void restore();
};

}

#endif
