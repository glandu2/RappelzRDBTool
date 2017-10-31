#ifndef PERSISTENTDATA_H
#define PERSISTENTDATA_H

#include <QVariant>

class QSettings;

class PersistentData {
public:
	PersistentData(QSettings* settings = 0, QString key = QString());
	PersistentData(const PersistentData& other);

	void setValue(const QVariant& val);
	QVariant value(const QVariant& defaultValue = QVariant());

	const QVariant& operator=(const QVariant& other) {
		setValue(other);
		return other;
	}
	const QString& operator=(const QString& other) {
		setValue(other);
		return other;
	}

protected:
	QString key;
	QSettings* settings;
};

#endif  // PERSISTENTDATA_H
