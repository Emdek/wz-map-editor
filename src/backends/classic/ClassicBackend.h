#ifndef WZMAPEDITOR_CLASSICBACKEND_H
#define WZMAPEDITOR_CLASSICBACKEND_H

#include <QtCore/QObject>

namespace WZMapEditor
{

class ClassicBackend : public QObject
{
	Q_OBJECT

public:
	explicit ClassicBackend(QObject *parent = NULL);

};

}

#endif
