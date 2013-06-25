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
	ShortcutEditorWidget(const QString &keySequence, int index, QWidget *parent = NULL);

	void updateDisplay();
	QKeySequence keySequence() const;
	int index();
	bool eventFilter(QObject *object, QEvent *event);

public slots:
	void clear();
	void setSequence(const QString &keySequence);

private:
	QKeySequence m_keySequence;
	QLineEdit *m_lineEdit;
	QToolButton *m_clearButton;
	QToolButton *m_restoreButton;
	QString m_recordedSequence;
	int m_index;
	bool m_isRecording;

signals:
	void restore();
};

}

#endif
