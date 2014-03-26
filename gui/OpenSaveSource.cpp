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
			message = tr("Enter full SQL target table (like Arcadia.dbo.StringResource)\nWarning: the table will be overwritten without prompt if it already exists !", "Save to SQL database inputbox label");
		else
			message = tr("Enter full SQL target table (like Arcadia.dbo.StringResource)", "Load from SQL database inputbox label");

		*sourceName = QInputDialog::getText(0, QCoreApplication::applicationName(), message, QLineEdit::Normal, defaultName, &ok);
		*sourceType = source;
		return ok;
	}

	QStringList filterList;
	QString filter;
	QString selectedFilter;

	QString allFiles = tr("All files (*)");
#ifdef _WIN32
	QString rdbFiles = tr("Client Database *.rdb;%1").arg(defaultName);
#else
	QString rdbFiles = tr("Client Database (*.rdb %1)").arg(defaultName);
#endif
	QString csvFiles = tr("Tabulation separated table (*.csv *.tsv *.txt)");
	QString sqlFiles = tr("SQL Script (*.sql)");

	if(autoDetectSourceType)
		filterList << allFiles;
	if(source == DST_RDB || autoDetectSourceType)
		filterList << rdbFiles;
	if(source == DST_CSV || autoDetectSourceType)
		filterList << csvFiles;
	if(save && (source == DST_SQLFile || autoDetectSourceType))
		filterList << sqlFiles;

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
		QString filename = QFileInfo(*sourceName).fileName();
		if(selectedFilter.contains(allFiles)) {
			if(filename.endsWith(".rdb", Qt::CaseInsensitive))
				*sourceType = DST_RDB;
			else if(filename.endsWith(".csv", Qt::CaseInsensitive) || filename.endsWith(".tsv", Qt::CaseInsensitive) || filename.endsWith(".txt", Qt::CaseInsensitive))
				*sourceType = DST_CSV;
			else if(save && filename.endsWith(".sql", Qt::CaseInsensitive))
				*sourceType = DST_SQLFile;
			else
				*sourceType = DST_RDB; //default, case of hashed names
		} else if(selectedFilter.contains(rdbFiles)) {
			*sourceType = DST_RDB;
		} else if(selectedFilter.contains(csvFiles)) {
			*sourceType = DST_CSV;
		} else if(save && selectedFilter.contains(sqlFiles)) {
			*sourceType = DST_SQLFile;
		} else {
			*sourceType = DST_CSV;
		}
	}

	return !sourceName->isNull();
}
