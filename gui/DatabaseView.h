#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include <QWidget>
#include <QModelIndex>
#include <QTimer>

#include "../Base/DataType.h"
#include "../Base/IDatabase.h"
#include "../Base/IDatabaseDescription.h"

#include "DatabaseDescriptionListModel.h"

class IRowManipulator;

namespace Ui {
class DatabaseView;
}

class DatabaseTableModel;
class QLabel;

class DatabaseView : public QWidget
{
		Q_OBJECT

	private:
		enum eToolStatus {
			TS_NoDbDescLoaded,
			TS_LoadingDbDesc,
			TS_NoDbLoaded,
			TS_LoadingDB,
			TS_DbLoaded,
			TS_SavingDB,
			TS_ClosingDB
		};
		
	public:
		DatabaseView(DatabaseDescriptionListModel* dbDescriptionListModel, QWidget *parent = 0);
		~DatabaseView();

		QString getDescriptionFileName() { return QString::fromLocal8Bit(db->getDatabaseDescription()->getFilename()); }
		QByteArray getDefaultFileName() { return QByteArray(db->getDatabaseDescription()->getDefaultFileName()); }
		QByteArray getDefaultTableName() { return QByteArray(db->getDatabaseDescription()->getDefaultTableName()); }

		const QString& getLoadedDatabaseName() { return loadedDatabaseName; }
		QLabel *getStatusLabel() { return statusBarLabel; }

		bool isSaved() { return savedData; }
		bool isDescriptionFileLoaded() { return db != NULL; }

	public slots:
		bool loadCloseDbDescriptionFile(bool isLoad);
		int loadDb(eDataSourceType type, QString filename, QString location = QString(), QString username = QString(), QString password = QString(), QByteArray options = QByteArray());
		int saveDb(eDataSourceType type, QString filename, QString location = QString(), QString username = QString(), QString password = QString(), QByteArray options = QByteArray());
		bool closeDb(bool force = false);
		void onSearch();
		void onChangeLocale(int newIndex);

	protected slots:
		void onModifyDb(QModelIndex, QModelIndex);
		void onCurrentDbDescChanged(int index);
		void onSearchResetStyle();

	signals:
		void titleChanged(DatabaseView* view);
		void onHighlightDbStruct(int index = -1);

	protected:
		void progressBarUpdate(int itemProceeded, int totalItem);
		static void DLLCALLCONV progressBarUpdateCallback(void *database, int itemProceeded, int totalItem);

		void setStatus(eToolStatus newStatus);
		void setDbDescButtonChecked(bool checked);
		void setWindowTitle(const QString &title);
		QString errorToString(eDataSourceType type, int error, bool save);

		
	private:
		Ui::DatabaseView *ui;
		IDatabase *db;
		bool savedData;
		QString loadedDatabaseName;
		DatabaseDescriptionListModel* dbDescriptionListModel;

		eToolStatus currentStatus;
		QLabel *statusBarLabel;

		DatabaseTableModel *databaseModel;

		int progressBarOffset;
		int lastPercentage;

		QTimer searchNotFoundStyleTimer;
};

#endif // DATABASEVIEW_H
