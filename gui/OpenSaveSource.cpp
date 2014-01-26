#include "OpenSaveSource.h"
#include <QFileDialog>
#include <QInputDialog>
#include <QCoreApplication>
#include <QString>

OpenSaveSource::OpenSaveSource(QSettings *settings)
{
	options = settings;
	autoDetectSourceType = true;

#ifdef __linux__
	defaultDirDLL = PersistentData(options, "RecentDirLinux/DatabaseDescription");
	defaultDirSourceOpen = PersistentData(options, "RecentDirLinux/SourceOpen");
	defaultDirSourceSave = PersistentData(options, "RecentDirLinux/SourceSave");
#else
	defaultDirDLL = PersistentData(options, "RecentDirWindows/DatabaseDescription");
	defaultDirSourceOpen = PersistentData(options, "RecentDirWindows/SourceOpen");
	defaultDirSourceSave = PersistentData(options, "RecentDirWindows/SourceSave");
#endif
}

void OpenSaveSource::setSourceType(eDataSourceType type) {
	autoDetectSourceType = false;
	source = type;
}

void OpenSaveSource::setAutoDetectSourceType() {
	autoDetectSourceType = true;
}

bool OpenSaveSource::getSource(bool save, QString *sourceName, eDataSourceType *sourceType) {
	eDataSourceType dummySource = DST_CSV;

	if(sourceType == 0)
		sourceType = &dummySource;

	if(autoDetectSourceType == false && (source == DST_SQLPostgres || source == DST_SQLServer)) {
		bool ok;
		QString message;

		if(save)
			message = "Enter full SQL target table (like Arcadia.dbo.StringResource)\nWarning: the table will be overwritten without prompt if it already exists !";
		else message = "Enter full SQL target table (like Arcadia.dbo.StringResource)";

        *sourceName = QInputDialog::getText(0, QCoreApplication::applicationName(), message, QLineEdit::Normal, QString("Arcadia.dbo."), &ok);
		*sourceType = source;
		return ok;
	}

	QStringList filterList;
	QString filter;
	QString selectedFilter;
	if(autoDetectSourceType)
		filterList << "All files (*.*)";
	if(source == DST_RDB || autoDetectSourceType)
		filterList << "Client Database (*.rdb)" << "Hashed Client Database file (*.*)";
	if(source == DST_CSV || autoDetectSourceType)
		filterList << "Tabulation separated table (*.csv *.tsv *.txt)";

	selectedFilter = filterList.first();
	filter = filterList.join(";;");

	if(save) {
		*sourceName = QFileDialog::getSaveFileName(0, tr("Choose Database Source"), defaultDirSourceSave.value(defaultDirSourceOpen.value()).toString(), filter, &selectedFilter);
		if(!sourceName->isNull())
			defaultDirSourceSave = QFileInfo(*sourceName).absolutePath();
	} else {
		*sourceName = QFileDialog::getOpenFileName(0, tr("Choose Database Source"), defaultDirSourceOpen.value().toString(), filter, &selectedFilter);
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
		else *sourceType = DST_CSV;
	}

	return !sourceName->isNull();
}
