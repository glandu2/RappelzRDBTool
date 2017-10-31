#include <QApplication>

#include "Maingui.h"
#include "Settings.h"
#include <QLocale>
#include <QMessageBox>
#include <QTextStream>
#include <QTranslator>
#include <version.h>

// Open files on command line

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);

	setlocale(LC_NUMERIC, "C");
	QLocale::setDefault(QLocale::c());

	QSettings options(QApplication::applicationDirPath() + "/RappelzRDBToolQt.ini", QSettings::IniFormat);
	Settings::setSettings(&options);

	QString translationFileName =
	    Settings::getSettings()
	        ->value("translationFile", QString("RappelzRDBToolQt.%1.qm").arg(QLocale::system().name()))
	        .toString();
	QTranslator translationFile;

	if(translationFile.load(translationFileName))
		Settings::getSettings()->setValue("translationFile", translationFileName);

	a.installTranslator(&translationFile);

	a.setApplicationName("RappelzRDBTool ~ Glandu2");
	a.setApplicationVersion(CPACK_PACKAGE_VERSION);

	Maingui mainGui;

	mainGui.show();

	a.exec();

	return 0;
}
