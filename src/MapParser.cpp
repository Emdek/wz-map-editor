#include "MapParser.h"
#include "MapInformation.h"


namespace WZMapEditor
{

MapParser::MapParser(QObject *parent) : QObject(parent),
	m_mapInformation(NULL)
{
}

MapInformation* MapParser::map()
{
	return m_mapInformation;
}

}
