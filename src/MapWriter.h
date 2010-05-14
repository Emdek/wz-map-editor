#ifndef MAPWRITER_H
#define MAPWRITER_H

#include <QtCore/QObject>


namespace WZMapEditor
{

class MapWriter : public QObject
{
	Q_OBJECT

public:
	MapWriter(QObject *parent = NULL);

};

}

#endif // MAPWRITER_H
