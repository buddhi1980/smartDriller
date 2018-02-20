#include "smartdrillermainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SmartDrillerMainWindow w;
	w.show();

	return a.exec();
}
