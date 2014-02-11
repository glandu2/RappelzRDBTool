#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
public:
	static QSettings *getSettings() { return options; }
	static void setSettings(QSettings *settings) { options = settings; }

private:
	static QSettings* options;
};

#endif // SETTINGS_H
