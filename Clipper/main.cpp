#include "clipper.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Clipper w;
	w.show();
	return a.exec();
}
