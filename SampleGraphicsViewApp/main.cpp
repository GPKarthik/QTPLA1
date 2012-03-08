#include "samplegraphicsviewapp.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SampleGraphicsViewApp w;
	w.showMaximized();
	return a.exec();
}
