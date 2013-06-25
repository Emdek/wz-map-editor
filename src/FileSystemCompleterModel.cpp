#include "FileSystemCompleterModel.h"

namespace WZMapEditor
{

FileSystemCompleterModel::FileSystemCompleterModel(QObject *parent) : QDirModel(parent)
{
	setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
}

QVariant FileSystemCompleterModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole && index.column() == 0)
	{
		QString path = QDir::toNativeSeparators(filePath(index));

		if (path.endsWith(QDir::separator()))
		{
			path.chop(1);
		}

		return path;
	}

	return QDirModel::data(index, role);
}

}
