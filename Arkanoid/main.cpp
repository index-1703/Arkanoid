#include <QApplication>
#include "Arkanoid.h"

int main(int argc, char* argv[]) {

	QApplication a(argc, argv);

	Arkanoid w;
	w.show();

	return a.exec();
}