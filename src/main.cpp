#include "MainWindow.h"

#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setApplicationName("WZMapEditor");
	application.setApplicationVersion("Pre-Alpha");
	application.setOrganizationName("Warzone2100");
	application.setOrganizationDomain("wz2100.net");

	QStringList arguments(QApplication::arguments());

	WZMapEditor::MainWindow mainWindow;

	for (int i = 0; i < arguments.count(); ++i)
	{
		if (QFileInfo(arguments.at(i)).absoluteFilePath() != QApplication::applicationFilePath() && QFile::exists(arguments.at(i)))
		{
			mainWindow.openFile(arguments.at(i));

			break;
		}
	}

	mainWindow.show();

	return application.exec();
}
