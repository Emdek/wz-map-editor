#include "MainWindow.h"

#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	WZMapEditor::MainWindow window;
	window.show();

	return application.exec();
}
