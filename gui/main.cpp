#include <QApplication>

#include <QMessageBox>
#include <QTextStream>
#include <QLocale>
#include "Maingui.h"
#include "Settings.h"

//Open files on command line

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	setlocale(LC_NUMERIC, "C");
	QLocale::setDefault(QLocale::c());


	QSettings options(QApplication::applicationDirPath() + "/RappelzRDBToolQt.ini", QSettings::IniFormat);
	Settings::setSettings(&options);

	a.setApplicationName("RappelzRDBTool by Glandu");
	a.setApplicationVersion("2.3.5");

	Maingui mainGui;

	mainGui.show();

	printf("Quitting ... %d\n", a.exec());

	return 0;
}
