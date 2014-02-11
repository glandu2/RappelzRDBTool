#include <QtGui/QApplication>
#include <QMessageBox>
#include <QTextStream>
#include "Maingui.h"
#include "Settings.h"

//Unicode mess:
//Combo box in sql table dialog to choose to which codepage convert data from SQL
//Combo box for locale to choose the codepage of files (and store string always as 8 bits wide strings)

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QSettings options(QApplication::applicationDirPath() + "/RappelzRDBToolQt.ini", QSettings::IniFormat);
	Settings::setSettings(&options);

	a.setApplicationName("RappelzRDBTool by Glandu");
	a.setApplicationVersion("2.0");

	Maingui mainGui;

	mainGui.show();

	printf("Quitting ... %d\n", a.exec());

	return 0;
}
