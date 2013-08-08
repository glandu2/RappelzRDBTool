#ifndef OPENSAVESOURCE_H
#define OPENSAVESOURCE_H

#include <QObject>
#include "../Base/DataType.h"
#include "PersistentData.h"

class QSettings;

class OpenSaveSource : public QObject
{
	public:
		OpenSaveSource(QSettings *settings);
		void setSourceType(eDataSourceType type);
		void setAutoDetectSourceType();
		bool getSource(bool save, QString *sourceName, eDataSourceType *sourceType);

	private:
		QSettings *options;
		bool autoDetectSourceType;
		eDataSourceType source;

		PersistentData defaultDirDLL;
		PersistentData defaultDirSourceOpen;
		PersistentData defaultDirSourceSave;

};

#endif // OPENSAVESOURCE_H
