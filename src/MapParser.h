#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <QtCore/QObject>


namespace WZMapEditor
{

class MapInformation;

class MapParser : public QObject
{
	Q_OBJECT

public:
	MapParser(QObject *parent = NULL);

	MapInformation* map();

private:
	MapInformation *m_mapInformation;

};

}

#endif // MAPPARSER_H
