#include <QApplication>

#include <QMessageBox>
#include <QTextStream>
#include "Maingui.h"
#include "Settings.h"

//Open files on command line

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QSettings options(QApplication::applicationDirPath() + "/RappelzRDBToolQt.ini", QSettings::IniFormat);
	Settings::setSettings(&options);

	a.setApplicationName("RappelzRDBTool by Glandu");
	a.setApplicationVersion("2.3.4");

	Maingui mainGui;

	mainGui.show();

	printf("Quitting ... %d\n", a.exec());

	return 0;
}
