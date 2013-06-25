#ifndef PREFERENCESMANAGER_H
#define PREFERENCESMANAGER_H

#include <QtCore/QObject>

class QAbstractButton;

namespace Ui
{
	class PreferencesDialog;
}

namespace WZMapEditor
{

class PreferencesManager : public QObject
{
	Q_OBJECT

public:
	PreferencesManager(QObject *parent = NULL);
	~PreferencesManager();

public slots:
	void selectDataPath();
	void save();
	void dialogButtonCliked(QAbstractButton *button);

private:
	Ui::PreferencesDialog *m_managerUi;
};

}

#endif
