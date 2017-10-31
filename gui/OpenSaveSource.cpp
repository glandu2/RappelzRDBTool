#include "OpenSaveSource.h"
#include <QFileDialog>
#include <QCoreApplication>
#include <QString>
#include "Settings.h"
#include "FileDialog.h"
#include "SqlOpenSaveDialog.h"

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

bool OpenSaveSource::getSource(bool save, QString defaultName, QString *sourceName, eDataSourceType *sourceType, QByteArray* options) {
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

		SqlOpenSaveDialog sqlDialog;
		sqlDialog.setMessage(message);
		sqlDialog.setTableName(defaultName);
		sqlDialog.setUseExistingTableSchemaEnabled(save); //enable use existing table schema only when saving

		ok = sqlDialog.exec() == QDialog::Accepted;

		*sourceName = sqlDialog.getTableName();
		*sourceType = source;
		if(sqlDialog.useExistingTableSchema())
			options->append("reusetable=;");
		return ok;
	}

	QStringList filterList;
	QString filter;
	QString selectedFilter;

	QString allFiles = tr("All files (*)", "File dialogbox filter");
	QString rdbFiles = tr("Client Database (*.rdb %1)", "File dialogbox filter").arg(defaultName);
	QString csvFiles = tr("Tabulation separated table (*.csv *.tsv *.txt)", "File dialogbox filter");
	QString sqlFiles = tr("SQL Script (*.sql)", "File dialogbox filter");

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
		*sourceName = FileDialog::getSaveFileName(0, tr("Choose Database Source", "File dialogbox title when saving"), defaultDirSourceSave.value(defaultDirSourceOpen.value(".")).toString() + "/" + defaultName, filter, &selectedFilter);
		if(!sourceName->isNull())
			defaultDirSourceSave = QFileInfo(*sourceName).absolutePath();
	} else {
		*sourceName = FileDialog::getOpenFileName(0, tr("Choose Database Source", "File dialogbox title when loading"), defaultDirSourceOpen.value(".").toString() + "/" + defaultName, filter, &selectedFilter);
		if(!sourceName->isNull())
			defaultDirSourceOpen = QFileInfo(*sourceName).absolutePath();
	}

	if(autoDetectSourceType) {
		QString filename = QFileInfo(*sourceName).fileName();
		if(selectedFilter.contains(allFiles)) {
			if(filename.endsWith(".rdb", Qt::CaseInsensitive) || filename.endsWith(".ref", Qt::CaseInsensitive))
				*sourceType = DST_RDB;
			else if(filename.endsWith(".csv", Qt::CaseInsensitive) || filename.endsWith(".tsv", Qt::CaseInsensitive) ||
			        filename.endsWith(".txt", Qt::CaseInsensitive))
				*sourceType = DST_CSV;
			else if(save && filename.endsWith(".sql", Qt::CaseInsensitive))
				*sourceType = DST_SQLFile;
			else
				*sourceType = DST_RDB;  // default, case of hashed names
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
