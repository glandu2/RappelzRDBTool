#include <QtGui/QApplication>
#include <QMessageBox>
#include <QTextStream>
#include "Maingui.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Maingui mainGui;

	a.setApplicationName("RappelzRDBTool by Glandu");
	a.setApplicationVersion("2.0");

	mainGui.show();

	printf("Quitting ... %d\n", a.exec());
	return 0;
}
