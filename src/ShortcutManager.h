#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <QtCore/QObject>

#include <QtGui/QAction>
#include <QtGui/QAbstractButton>

namespace Ui
{
	class ShortcutEditorDialog;
}

namespace WZMapEditor
{

class MainWindow;

class ShortcutManager : public QObject
{
	Q_OBJECT

public:
	ShortcutManager(QList<QAction*> actions, MainWindow *parent = NULL);
	~ShortcutManager();

	QString restoreDefaultShortcut(int index);
	bool checkSequence(const QKeySequence &sequence, int index);

public slots:
	void filter(const QString &filter);
	void save();
	void dialogButtonCliked(QAbstractButton *button);

private:
	Ui::ShortcutEditorDialog *m_managerUi;
	MainWindow *m_mainWindow;
	QList<QAction*> m_actions;
};

}

#endif // SHORTCUTMANAGER_H
