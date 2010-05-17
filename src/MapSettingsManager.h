#ifndef MAPSETTINGSMANAGER_H
#define MAPSETTINGSMANAGER_H

#include <QtCore/QObject>


namespace Ui
{
	class MapSettingsDialog;
}

namespace WZMapEditor
{

class MapInformation;

class MapSettingsManager : public QObject
{
	Q_OBJECT

public:
	MapSettingsManager(MapInformation *mapInformation, QObject *parent = NULL);
	~MapSettingsManager();

public slots:
	void changeTileset(int index);
	void save();

private:
	MapInformation *m_mapInformation;
	Ui::MapSettingsDialog *m_managerUi;
};

}

#endif // MAPSETTINGSMANAGER_H
