#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <QtCore/QObject>

namespace WZMapEditor
{

class MapParser : public QObject
{
	Q_OBJECT

public:
	MapParser(QObject *parent = NULL);

};

}

#endif // MAPPARSER_H
