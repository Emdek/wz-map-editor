#ifndef MAPSETTINGSMANAGER_H
#define MAPSETTINGSMANAGER_H

#include <QtCore/QObject>


namespace Ui
{
	class MapSettingsDialog;
}

namespace WZMapEditor
{

class Map;

class MapSettingsManager : public QObject
{
	Q_OBJECT

public:
	MapSettingsManager(Map *map, bool newMap, QObject *parent = NULL);
	~MapSettingsManager();

public slots:
	void changeTileset(int index);
	void save();

private:
	Map *m_map;
	bool m_newMap;
	Ui::MapSettingsDialog *m_managerUi;
};

}

#endif // MAPSETTINGSMANAGER_H
