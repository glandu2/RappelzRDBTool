#ifndef MAINGUI_H
#define MAINGUI_H

#include <QMainWindow>
#include <QModelIndex>
#include "PersistentData.h"
#include "HashConverterDialog.h"

class IDatabase;
class IRowManipulator;
class IDatabaseDescription;

namespace Ui {
class MainWindow;
}

class QLabel;
class SqlConfigDialog;
class QSettings;
class DatabaseView;
class DatabaseDescriptionListModel;
class TabBarEventFilter;
class DatabaseDescManageDialog;

class Maingui : public QMainWindow
{
		Q_OBJECT

	public:
		enum eSourceType {
			ST_None,
			ST_Rdb,
			ST_Csv,
			ST_Sql,
			ST_SqlDatabase
		};

		explicit Maingui(QWidget *parent = 0);
		~Maingui();

	public slots:
		void onLoadDbStructDLL();

		void onLoadFile();
		void onLoadRDB();
		void onLoadCSV();
		void onLoadSQLDatabase();

		void onSaveFile();
		void onSaveRDB();
		void onSaveCSV();
		void onSaveSQL();
		void onSaveSQLDatabase();

		void onCloseDb();

		void onSQLOptions();
		void onDbDescManage();
		void onToggleHashConverterDialog();

		void onAbout();

	protected slots:
		void onTabChanged(int index);
		void onAddTab();
		void onRemoveTab(int index = -1);
		void onViewTitleChanged(DatabaseView* view);
		void onDbStructHighlighted(int index);

	protected:
		void loadSaveDbFile(bool save, eSourceType srcType, bool hashedFilename);
		virtual void closeEvent(QCloseEvent *event);

	private:
		DatabaseDescriptionListModel *dbDescriptionModel;
		QList<DatabaseView*> databaseViews;

		Ui::MainWindow *ui;
		SqlConfigDialog *sqlConfigDialog;
		DatabaseDescManageDialog *dbDescriptionManageDialog;

		TabBarEventFilter *tabEventFilter;
		QLabel *currentStatusBarLabel;

		HashConverterDialog hashConverter;
};

#endif // MAINGUI_H
