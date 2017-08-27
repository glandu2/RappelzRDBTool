#ifndef OPENSAVESOURCE_H
#define OPENSAVESOURCE_H

#include <QObject>
#include "DataType.h"
#include "PersistentData.h"

class OpenSaveSource : public QObject
{
	Q_OBJECT
	public:
		OpenSaveSource();
		void setSourceType(eDataSourceType type);
		void setAutoDetectSourceType();
		bool getSource(bool save, QString defaultName, QString *sourceName, eDataSourceType *sourceType, QByteArray* options);

	private:
		bool autoDetectSourceType;
		eDataSourceType source;

		PersistentData defaultDirDLL;
		PersistentData defaultDirSourceOpen;
		PersistentData defaultDirSourceSave;

};

#endif // OPENSAVESOURCE_H
