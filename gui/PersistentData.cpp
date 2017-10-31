#include "PersistentData.h"
#include <QSettings>

PersistentData::PersistentData(QSettings* settings, QString key) : key(key), settings(settings) {}

PersistentData::PersistentData(const PersistentData& other) {
	key = other.key;
	settings = other.settings;
}

void PersistentData::setValue(const QVariant& val) {
	if(settings)
		settings->setValue(key, val);
}

QVariant PersistentData::value(const QVariant& defaultValue) {
	if(settings)
		return settings->value(key, defaultValue);
	else
		return QVariant();
}
