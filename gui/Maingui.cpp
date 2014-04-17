#include "Maingui.h"
#include "ui_Maingui.h"

#include "../Base/IDatabase.h"
#include "../Base/IRowManipulator.h"
#include "DatabaseDescriptionListModel.h"
#include "DatabaseTableModel.h"
#include "SqlConfigDialog.h"
#include "DatabaseView.h"
#include "OpenSaveSource.h"
#include "TabBarEventFilter.h"
#include "DatabaseDescManageDialog.h"
#include "NameToHash.h"
#include "Settings.h"

#include <QMessageBox>
#include <QStringList>
#include <QSettings>
#include <QLabel>
#include <QCloseEvent>

Maingui::Maingui(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	hashConverter(this)
{
	ui->setupUi(this);


	tabEventFilter = new TabBarEventFilter(ui->databaseTab, this);

	sqlConfigDialog = new SqlConfigDialog();
	dbDescriptionModel = new DatabaseDescriptionListModel();
	dbDescriptionManageDialog = new DatabaseDescManageDialog(dbDescriptionModel, this);

	ui->actionUse_hashed_files->setChecked(Settings::getSettings()->value("useHashedFilename").toBool());

	currentStatusBarLabel = 0;

	connect(ui->actionNew_Tab, SIGNAL(triggered()), this, SLOT(onAddTab()));
	connect(ui->actionClose_Tab, SIGNAL(triggered()), this, SLOT(onRemoveTab()));

	connect(ui->actionManage_Db_Description_files, SIGNAL(triggered()), this, SLOT(onDbDescManage()));

	connect(ui->actionLoad_DB_Structure_DLL, SIGNAL(triggered()), this, SLOT(onLoadDbStructDLL()));
	connect(ui->actionLoadFromFile, SIGNAL(triggered()), this, SLOT(onLoadFile()));
	connect(ui->actionLoad_from_RDB, SIGNAL(triggered()), this, SLOT(onLoadRDB()));
	connect(ui->actionLoad_from_CSV, SIGNAL(triggered()), this, SLOT(onLoadCSV()));
	connect(ui->actionLoad_from_SQLDatabase, SIGNAL(triggered()), this, SLOT(onLoadSQLDatabase()));
	connect(ui->actionSaveToFile, SIGNAL(triggered()), this, SLOT(onSaveFile()));
	connect(ui->actionSave_to_RDB, SIGNAL(triggered()), this, SLOT(onSaveRDB()));
	connect(ui->actionSave_to_CSV, SIGNAL(triggered()), this, SLOT(onSaveCSV()));
	connect(ui->actionSave_to_SQL_file, SIGNAL(triggered()), this, SLOT(onSaveSQL()));
	connect(ui->actionSave_to_SQLDatabase, SIGNAL(triggered()), this, SLOT(onSaveSQLDatabase()));
	connect(ui->actionClose_Database, SIGNAL(triggered()), this, SLOT(onCloseDb()));
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui->actionSQL_Options, SIGNAL(triggered()), this, SLOT(onSQLOptions()));
	connect(ui->actionShow_Hide_Hash_Converter, SIGNAL(triggered()), this, SLOT(onToggleHashConverterDialog()));

	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(onAbout()));
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

	connect(ui->databaseTab, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
	connect(tabEventFilter, SIGNAL(addTab()), this, SLOT(onAddTab()));
	connect(tabEventFilter, SIGNAL(removeTab(int)), this, SLOT(onRemoveTab(int)));


	onAddTab();  // Have at least one tab
}

Maingui::~Maingui()
{
	Settings::getSettings()->setValue("useHashedFilename", ui->actionUse_hashed_files->isChecked());
	delete dbDescriptionModel;
	delete sqlConfigDialog;
	delete ui;
}

void Maingui::onTabChanged(int index) {
	DatabaseView *currentView = databaseViews.value(index, 0);
	if(!currentView)
		return;

	if(currentStatusBarLabel)
		ui->statusBar->removeWidget(currentStatusBarLabel);
	currentStatusBarLabel = currentView->getStatusLabel();

	if(currentStatusBarLabel) {
		ui->statusBar->addWidget(currentStatusBarLabel);
		currentStatusBarLabel->show();
	}
}

void Maingui::onAddTab() {
	int index;
	DatabaseView *tabView = new DatabaseView(dbDescriptionModel, ui->databaseTab);
	connect(tabView, SIGNAL(titleChanged(DatabaseView*)), this, SLOT(onViewTitleChanged(DatabaseView*)));
	connect(tabView, SIGNAL(onHighlightDbStruct(int)), this, SLOT(onDbStructHighlighted(int)));

	ui->databaseTab->setUpdatesEnabled(false);
	index = ui->databaseTab->addTab(tabView, tabView->windowTitle());
	ui->databaseTab->setTabToolTip(index, tabView->getLoadedDatabaseName());
	ui->databaseTab->setUpdatesEnabled(true);

	databaseViews.insert(index, tabView);


	if(ui->databaseTab->currentIndex() == index)
		onTabChanged(index);
	else ui->databaseTab->setCurrentIndex(index);
}

void Maingui::onRemoveTab(int index) {
	if(index < 0)
		index = ui->databaseTab->currentIndex();

	DatabaseView *tabView = databaseViews.at(index);

	if(tabView->closeDb() == false)
		return;

	if(currentStatusBarLabel == tabView->getStatusLabel()) {
		ui->statusBar->removeWidget(currentStatusBarLabel);
		currentStatusBarLabel = 0;
	}

	ui->databaseTab->removeTab(index);
	databaseViews.removeAt(index);


	delete tabView;

	if(ui->databaseTab->count() == 0)
		onAddTab();  //Always have at least 1 tab
}

void Maingui::onViewTitleChanged(DatabaseView* view) {
	int i;

	for(i = 0; i < databaseViews.size(); i++) {
		if(databaseViews.at(i) == view) {
			ui->databaseTab->setTabText(i, view->windowTitle());
			ui->databaseTab->setTabToolTip(i, view->getLoadedDatabaseName());
			break;
		}
	}
}

void Maingui::onDbStructHighlighted(int index) {
	if(index < 0)
		statusBar()->clearMessage();
	else statusBar()->showMessage(QString(dbDescriptionModel->getDbDescription(index)->getFilename()), 5000);
}

void Maingui::onLoadDbStructDLL() {
	DatabaseView *currentView = databaseViews.value(ui->databaseTab->currentIndex(), 0);

	if(currentView == 0)
		return;

	currentView->loadCloseDbDescriptionFile(true);
}

void Maingui::loadSaveDbFile(bool save, eSourceType srcType) {
	OpenSaveSource *openSaveSource;
	bool ok;
	bool autoDetectType;
	bool hashedFilename = ui->actionUse_hashed_files->isChecked();
	QString sourceName;
	QByteArray defaultSourceName;
	eDataSourceType sourceType;
	DatabaseView *currentView = databaseViews.value(ui->databaseTab->currentIndex(), 0);
	QByteArray options;

	if(currentView == 0)
		return;

	//Load the current select db structure if not already done
	onLoadDbStructDLL();

	if(srcType == ST_SqlDatabase && sqlConfigDialog->getServerName().isEmpty()) {
		sqlConfigDialog->exec();
		if(sqlConfigDialog->getServerName().isEmpty())
			return;
	}

	if(!save && currentView->isSaved() == false) {
		QMessageBox::StandardButton button;
		button = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The database %1 is not saved.\n\nContinue ?", "User wants to load another database but the current is modified and not saved").arg(currentView->getLoadedDatabaseName()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
		if(button == QMessageBox::No)
			return;
	}

	autoDetectType = false;
	switch(srcType) {
		case ST_None:
			autoDetectType = true;
			defaultSourceName = currentView->getDefaultFileName() + ".rdb";
			if(hashedFilename) {
				QByteArray hashed(defaultSourceName.size() + 2, 0);
				convertNameToHash(defaultSourceName.constData(), hashed.data(), LEGACY_SEED);
				defaultSourceName = hashed;
			}
			break;

		case ST_Rdb:
			sourceType = DST_RDB;
			defaultSourceName = currentView->getDefaultFileName() + ".rdb";
			if(hashedFilename) {
				QByteArray hashed(defaultSourceName.size() + 2, 0);
				convertNameToHash(defaultSourceName.constData(), hashed.data(), LEGACY_SEED);
				defaultSourceName = hashed;
			}

			break;

		case ST_Csv:
			sourceType = DST_CSV;
			defaultSourceName = currentView->getDefaultFileName() + ".csv";
			break;

		case ST_Sql:
			sourceType = DST_SQLFile;
			defaultSourceName = QByteArray("Arcadia.dbo.") + currentView->getDefaultTableName() + ".sql";
			break;

		case ST_SqlDatabase:
			sourceType = sqlConfigDialog->getServerType();
			if(sourceType == DST_SQLServer)
				defaultSourceName = QByteArray("Arcadia.dbo.") + currentView->getDefaultTableName();
			else
				defaultSourceName = QByteArray("Arcadia.") + currentView->getDefaultTableName();
			break;
	}

	openSaveSource = new OpenSaveSource();

	if(autoDetectType)
		openSaveSource->setAutoDetectSourceType();
	else
		openSaveSource->setSourceType(sourceType);

	ok = openSaveSource->getSource(save, defaultSourceName, &sourceName, &sourceType, &options);

	delete openSaveSource;

	if(ok) {
		if(!save) currentView->closeDb(true);

		if(!save)
			currentView->loadDb(sourceType, sourceName, sqlConfigDialog->getServerName(), sqlConfigDialog->getUsername(), sqlConfigDialog->getPassword(), options);
		else currentView->saveDb(sourceType, sourceName, sqlConfigDialog->getServerName(), sqlConfigDialog->getUsername(), sqlConfigDialog->getPassword(), options);
	}
}

void Maingui::onCloseDb() {
	DatabaseView *currentView = databaseViews.value(ui->databaseTab->currentIndex(), 0);

	if(currentView)
		currentView->closeDb();
}

void Maingui::closeEvent(QCloseEvent *event)
{
	for(int i = 0; i < databaseViews.size(); i++) {
		DatabaseView *currentView = databaseViews.value(i, 0);
		if(!currentView->isSaved()) {
			ui->databaseTab->setCurrentIndex(i);
			QMessageBox::StandardButton retButton;
			retButton = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The database %1 is not saved.\n\nContinue ?", "User want to quit but there is at least one database modified but not saved").arg(currentView->getLoadedDatabaseName()), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if(retButton != QMessageBox::Yes) {
				event->ignore();
				return;
			}
		}
	}
	event->accept();
}

void Maingui::onLoadFile() {
	loadSaveDbFile(false, ST_None);
}

void Maingui::onLoadRDB() {
	loadSaveDbFile(false, ST_Rdb);
}

void Maingui::onLoadCSV() {
	loadSaveDbFile(false, ST_Csv);
}

void Maingui::onLoadSQLDatabase() {
	loadSaveDbFile(false, ST_SqlDatabase);
}


void Maingui::onSaveFile() {
	loadSaveDbFile(true, ST_None);
}

void Maingui::onSaveRDB() {
	loadSaveDbFile(true, ST_Rdb);
}

void Maingui::onSaveCSV() {
	loadSaveDbFile(true, ST_Csv);
}

void Maingui::onSaveSQL() {
	loadSaveDbFile(true, ST_Sql);
}

void Maingui::onSaveSQLDatabase() {
	loadSaveDbFile(true, ST_SqlDatabase);
}

void Maingui::onSQLOptions() {
	sqlConfigDialog->exec();
}

void Maingui::onDbDescManage() {
	dbDescriptionManageDialog->exec();
}

void Maingui::onToggleHashConverterDialog() {
	if(hashConverter.isHidden())
		hashConverter.show();
	else
		hashConverter.hide();
}

void Maingui::onAbout() {
	QMessageBox::information(this, QCoreApplication::applicationName(), QString("This tool can convert RDB files to CSV files and CSV to RDB. It was made by Glandu2 for epvp.\nVersion: %1\nUsing locale: %2").arg(QCoreApplication::applicationVersion()).arg(QLocale::system().name()));
}
