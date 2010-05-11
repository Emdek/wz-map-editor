#ifndef SHORTCUTMANAGER_H
#define SHORTCUTMANAGER_H

#include <QtCore/QObject>


class QAbstractButton;
class QKeySequence;

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
	ShortcutManager(MainWindow *parent = NULL);
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
};

}

#endif // SHORTCUTMANAGER_H
