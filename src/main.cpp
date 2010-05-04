#include "MainWindow.h"

#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	application.setApplicationName("WZMapEditor");
	application.setApplicationVersion("Pre-Alpha");
	application.setOrganizationName("Warzone2100");
	application.setOrganizationDomain("wz2100.net");

	WZMapEditor::MainWindow mainWindow;
	mainWindow.show();

	return application.exec();
}
