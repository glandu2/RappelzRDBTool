#include "OpenSaveSource.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QCoreApplication>
#include <QString>
#include "Settings.h"

OpenSaveSource::OpenSaveSource()
{
	autoDetectSourceType = true;
	source = DST_RDB;

#ifdef __linux__
	defaultDirDLL = PersistentData(Settings::getSettings(), "RecentDirLinux/DatabaseDescription");
	defaultDirSourceOpen = PersistentData(Settings::getSettings(), "RecentDirLinux/SourceOpen");
	defaultDirSourceSave = PersistentData(Settings::getSettings(), "RecentDirLinux/SourceSave");
#else
	defaultDirDLL = PersistentData(Settings::getSettings(), "RecentDirWindows/DatabaseDescription");
	defaultDirSourceOpen = PersistentData(Settings::getSettings(), "RecentDirWindows/SourceOpen");
	defaultDirSourceSave = PersistentData(Settings::getSettings(), "RecentDirWindows/SourceSave");
#endif
}

void OpenSaveSource::setSourceType(eDataSourceType type) {
	autoDetectSourceType = false;
	source = type;
}

void OpenSaveSource::setAutoDetectSourceType() {
	autoDetectSourceType = true;
}

bool OpenSaveSource::getSource(bool save, QString defaultName, QString *sourceName, eDataSourceType *sourceType) {
	eDataSourceType dummySource = DST_CSV;

	if(sourceType == 0)
		sourceType = &dummySource;

	if(autoDetectSourceType == false && (source == DST_SQLPostgres || source == DST_SQLServer)) {
		bool ok;
		QString message;

		if(save)
			message = "Enter full SQL target table (like Arcadia.dbo.StringResource)\nWarning: the table will be overwritten without prompt if it already exists !";
		else message = "Enter full SQL target table (like Arcadia.dbo.StringResource)";

		*sourceName = QInputDialog::getText(0, QCoreApplication::applicationName(), message, QLineEdit::Normal, defaultName, &ok);
		*sourceType = source;
		return ok;
	}

	QStringList filterList;
	QString filter;
	QString selectedFilter;
	if(source == DST_RDB || autoDetectSourceType)
#ifdef _WIN32
		filterList << "Client Database *.rdb;" + defaultName;
#else
		filterList << "Client Database (*.rdb " + defaultName + ")";
#endif
	if(source == DST_CSV || autoDetectSourceType)
		filterList << "Tabulation separated table (*.csv *.tsv *.txt)";
	if(save && (source == DST_SQLFile || autoDetectSourceType))
		filterList << "SQL Script (*.sql)";
	if(autoDetectSourceType)
		filterList << "All files (*.*)";

	selectedFilter = filterList.first();
	filter = filterList.join(";;");

	if(save) {
		*sourceName = QFileDialog::getSaveFileName(0, tr("Choose Database Source"), defaultDirSourceSave.value(defaultDirSourceOpen.value()).toString() + "/" + defaultName, filter, &selectedFilter);
		if(!sourceName->isNull())
			defaultDirSourceSave = QFileInfo(*sourceName).absolutePath();
	} else {
		*sourceName = QFileDialog::getOpenFileName(0, tr("Choose Database Source"), defaultDirSourceOpen.value().toString() + "/" + defaultName, filter, &selectedFilter);
		if(!sourceName->isNull())
			defaultDirSourceOpen = QFileInfo(*sourceName).absolutePath();
	}

	if(autoDetectSourceType) {
		bool useExtension = false;
		QString filename = QFileInfo(*sourceName).fileName();
		if(selectedFilter.contains("All files"))
			useExtension = true;

		if((useExtension && filename.endsWith(".rdb", Qt::CaseInsensitive)) || selectedFilter.contains("Client Database"))
			*sourceType = DST_RDB;
		else if((useExtension && filename.endsWith(".csv", Qt::CaseInsensitive)) || selectedFilter.contains("*.csv"))
			*sourceType = DST_CSV;
		else if(save && ((useExtension && filename.endsWith(".sql", Qt::CaseInsensitive)) || selectedFilter.contains("*.sql")))
			*sourceType = DST_SQLFile;
		else *sourceType = DST_CSV;
	}

	return !sourceName->isNull();
}
