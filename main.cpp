#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <qdesktopwidget.h>

const int SCR_WIDTH = 1600;
const int SCR_HEIGHT = 1080;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow window;
	window.resize(window.sizeHint());
	int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();
	int widgetArea = window.width() * window.height();

	window.setWindowTitle("OpenGL with Qt");
	window.resize(SCR_WIDTH, SCR_HEIGHT);
	//if (((float)widgetArea / (float)desktopArea) < 0.75f)
	//	window.show();
	//else
	//	window.showMaximized();
	window.show();
	return app.exec();
}
