#include <QApplication>

#include <QMessageBox>
#include <QTextStream>
#include <QLocale>
#include <QTranslator>
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

	QString translationFileName = Settings::getSettings()->value("translationFile", QString("RappelzRDBToolQt.%1.qm").arg(QLocale::system().name())).toString();
	QTranslator translationFile;

	if(translationFile.load(translationFileName))
		Settings::getSettings()->setValue("translationFile", translationFileName);

	a.installTranslator(&translationFile);

	a.setApplicationName("RappelzRDBTool by Glandu");
	a.setApplicationVersion("2.4.0");

	Maingui mainGui;

	mainGui.show();

	a.exec();

	return 0;
}
