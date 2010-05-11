#ifndef FILESYSTEMCOMPLETERMODEL_H
#define FILESYSTEMCOMPLETERMODEL_H

#include <QtGui/QDirModel>


namespace WZMapEditor
{

class FileSystemCompleterModel : public QDirModel
{
public:
	FileSystemCompleterModel(QObject *parent = NULL);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

}

#endif // FILESYSTEMCOMPLETERMODEL_H
