#include "DatabaseView.h"
#include "ui_DatabaseView.h"
#include "DatabaseTableModel.h"
#include "../Base/IDatabase.h"
#include "../Base/ICharsetConverter.h"
#include "Settings.h"

#include <QMessageBox>
#include <QFileInfo>
#include <QDate>

bool sortStringCaseInsensitive( QByteArray a, QByteArray b ) {
	return qstricmp(a.constData(), b.constData()) < 0;
}

DatabaseView::DatabaseView(DatabaseDescriptionListModel *dbDescriptionListModel, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DatabaseView),
	dbDescriptionListModel(dbDescriptionListModel)
{
	ui->setupUi(this);
	progressBarOffset = 0;
	lastPercentage = -1;
	db = NULL;
	currentStatus = TS_NoDbDescLoaded;

	const struct CharsetInfo * codepageList = availableCharsets();
	QString defaultCodec = Settings::getSettings()->value("charset", "CP1252").toString();
	for(int i = 0; codepageList[i].name; i++) {
		ui->localeCombo->insertItem(i, QString::fromLatin1(codepageList[i].description), QString::fromLatin1(codepageList[i].name));
		if(QString::fromLatin1(codepageList[i].name) == defaultCodec)
			ui->localeCombo->setCurrentIndex(i);
	}

	databaseModel = new DatabaseTableModel(ui->localeCombo->itemData(ui->localeCombo->currentIndex()).toByteArray(), this);
	statusBarLabel = new QLabel(this);
	statusBarLabel->hide();
	setStatus(TS_NoDbDescLoaded);

	ui->databaseTable->setModel(databaseModel);
	ui->progressBar->reset();
	ui->dbStructCombo->setModel(dbDescriptionListModel);

	ui->dbLoadClose->setMinimumWidth(ui->dbLoadClose->sizeHint().width());
	setDbDescButtonChecked(false);

	savedData = true;
	loadedDatabaseName = tr("<No database>", "tab text when no database loaded");
	setWindowTitle(loadedDatabaseName);

	searchNotFoundStyleTimer.setInterval(2000);
	searchNotFoundStyleTimer.setSingleShot(true);

	connect(&searchNotFoundStyleTimer, SIGNAL(timeout()), this, SLOT(onSearchResetStyle()));

	connect(ui->localeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeLocale(int)));

	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(onSearch()));
	connect(ui->searchTextEdit, SIGNAL(returnPressed()), this, SLOT(onSearch()));
	connect(databaseModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onModifyDb(QModelIndex, QModelIndex)));
	connect(ui->dbLoadClose, SIGNAL(clicked(bool)), this, SLOT(loadCloseDbDescriptionFile(bool)));
	connect(ui->dbStructCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentDbDescChanged(int)));
	connect(ui->dbStructCombo, SIGNAL(highlighted(int)), this, SIGNAL(onHighlightDbStruct(int)));
	connect(ui->dbStructCombo, SIGNAL(activated(int)), this, SIGNAL(onHighlightDbStruct()));
}

DatabaseView::~DatabaseView()
{
	Settings::getSettings()->setValue("charset", ui->localeCombo->itemData(ui->localeCombo->currentIndex()));
	delete ui;
	if(db)
		db->destroy();
}

void DatabaseView::setDbDescButtonChecked(bool checked) {
	ui->dbLoadClose->setChecked(checked);
	if(checked)
		ui->dbLoadClose->setText(tr("Loaded", "Database description dll button label"));
	else
		ui->dbLoadClose->setText(tr("Load", "Database description dll button label"));
}

bool DatabaseView::loadCloseDbDescriptionFile(bool isLoad) {
	//Db description already loaded, nothing to do
	if(isLoad && db && db->getDatabaseDescription() == dbDescriptionListModel->getDbDescription(ui->dbStructCombo->currentIndex()))
		return true;

	if(db != NULL) {
		if(closeDb() == false) {
			ui->dbLoadClose->setChecked(false);
			return false;
		}

		db->destroy();
		db = NULL;
		setStatus(TS_NoDbDescLoaded);
	}

	if(isLoad) {
		if(ui->dbStructCombo->currentIndex() == -1) {
			QMessageBox::warning(this, QCoreApplication::applicationName(), tr("You must add Database Description files first !", "Error when there is no db description files but the user attempt to load one"));
			setDbDescButtonChecked(false);
			return false;
		}

		setStatus(TS_LoadingDbDesc);
		IDatabaseDescription* dbDescription = dbDescriptionListModel->getDbDescription(ui->dbStructCombo->currentIndex());

		db = createDatabase(dbDescription);
		if(!db) {
			ui->dbLoadClose->setChecked(false);
			setStatus(TS_NoDbDescLoaded);
			return false;
		}
		db->setUserData(this);
		setStatus(TS_NoDbLoaded);
	}

	setDbDescButtonChecked(isLoad);
	return true;
}

void DatabaseView::onCurrentDbDescChanged(int index) {
	IDatabaseDescription* dbDescription = dbDescriptionListModel->getDbDescription(index);

	if(!db)
		return;

	if(dbDescription == db->getDatabaseDescription())
		setDbDescButtonChecked(true);
	else
		setDbDescButtonChecked(false);
}

void DatabaseView::progressBarUpdateCallback(void *database, int itemProceeded, int totalItem) {
	DatabaseView *dbView = reinterpret_cast<DatabaseView*>(database);

	dbView->progressBarUpdate(itemProceeded, totalItem);
}

void DatabaseView::progressBarUpdate(int itemProceeded, int totalItem) {
	int newPercentage;

	if(!totalItem) return;

	newPercentage = itemProceeded*100/totalItem;

	if(newPercentage != lastPercentage) {
		ui->progressBar->setValue(progressBarOffset + newPercentage);
		lastPercentage = newPercentage;

		QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
	}
}

int DatabaseView::loadDb(eDataSourceType type, QString filename, QString location, QString username, QString password, QByteArray options) {
	int result;
	QByteArray locationStr;
	QByteArray usernameStr;
	QByteArray passwordStr;

	options += QByteArray("charset=") + ui->localeCombo->itemData(ui->localeCombo->currentIndex()).toByteArray() + ";";

	setStatus(TS_LoadingDB);

	ui->progressBar->setValue(0);
	if(db->getRowCount() > 0) {
		ui->progressBar->setMaximum(200);
		progressBarOffset = 0;

		if(closeDb() == false) {
			ui->progressBar->setMaximum(100);
			return 0;
		}

		progressBarOffset = 100;
	} else {
		ui->progressBar->setMaximum(100);
		progressBarOffset = 0;
	}

	if(!location.isEmpty())
		locationStr = location.toLocal8Bit();

	if(!username.isEmpty())
		usernameStr = username.toLocal8Bit();

	if(!password.isEmpty())
		passwordStr = password.toLocal8Bit();

	result = db->readData(type, filename.toLocal8Bit().constData(), &progressBarUpdateCallback, this, locationStr, usernameStr, passwordStr, options.constData());
	savedData = true;

	if(result != 0) {
		closeDb();
		setStatus(TS_NoDbLoaded);
		ui->progressBar->reset();
		QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Couldn't load the database file: %1(%2)", "Load error global message in messagebox").arg(errorToString(type, result, false)).arg(result));
	} else {
		loadedDatabaseName = filename;
		setWindowTitle(loadedDatabaseName);

		databaseModel->bindToDatabase(db);

		ui->databaseTable->setEnabled(true);

		ui->searchColumnCombo->clear();
		for(int i = 0; i<databaseModel->columnCount(); i++) {
			ui->searchColumnCombo->addItem(databaseModel->headerData(i, Qt::Horizontal).toString());
		}

		ui->progressBar->setValue(100);
		setStatus(TS_DbLoaded);
	}
	ui->progressBar->setMaximum(100);
	progressBarOffset = 0;

	return result;
}

int DatabaseView::saveDb(eDataSourceType type, QString filename, QString location, QString username, QString password, QByteArray options) {
	int result;
	QByteArray locationStr;
	QByteArray usernameStr;
	QByteArray passwordStr;

	options += QByteArray("charset=") + ui->localeCombo->itemData(ui->localeCombo->currentIndex()).toByteArray() + ";";

	setStatus(TS_SavingDB);

	ui->progressBar->setValue(0);
	ui->databaseTable->setEnabled(false);

	if(!location.isEmpty())
		locationStr = location.toLocal8Bit();

	if(!username.isEmpty())
		usernameStr = username.toLocal8Bit();

	if(!password.isEmpty())
		passwordStr = password.toLocal8Bit();


	result = db->writeData(type, filename.toLocal8Bit().constData(), &progressBarUpdateCallback, this, locationStr.constData(), usernameStr.constData(), passwordStr.constData(), options.constData());

	setStatus(TS_DbLoaded);

	if(result != 0) {
		ui->progressBar->reset();
		QMessageBox::warning(this, QCoreApplication::applicationName(), tr("Couldn't save the database file: %1(%2)", "Save error global message in messagebox").arg(errorToString(type, result, true)).arg(result));
	} else {
		savedData = true;
		loadedDatabaseName = filename;
		setWindowTitle(loadedDatabaseName);
	}
	ui->databaseTable->setEnabled(true);

	return result;
}

bool DatabaseView::closeDb(bool force) {
	if(db) {
		if(!force && savedData == false) {
			QMessageBox::StandardButton button;
			button = QMessageBox::warning(this, QCoreApplication::applicationName(), tr("The database %1 is not saved.\n\nContinue ?", "User closes unsaved database").arg(loadedDatabaseName), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
			if(button == QMessageBox::No)
				return false;
		}

		setStatus(TS_ClosingDB);
		ui->databaseTable->setEnabled(false);
		databaseModel->unbindDatabase();
		db->close(&progressBarUpdateCallback, this);
		progressBarUpdate(100, 100);
		loadedDatabaseName = tr("<No database>", "tab text when no database loaded");
		setWindowTitle(loadedDatabaseName);
		setStatus(TS_NoDbLoaded);
		savedData = true;
	}

	return true;
}

void DatabaseView::onSearch() {
	if(!db || db->getRowCount() < 1)
		return;

	int i, column = ui->searchColumnCombo->currentIndex(), rowCount = databaseModel->rowCount();
	bool found = false;
	QModelIndexList selection = ui->databaseTable->selectionModel()->selectedIndexes();


	if(selection.isEmpty())
		i = 0;
	else i = selection.first().row()+1; //allow several search without changing selection with mouse
	for(; i<rowCount; i++) {
		QModelIndex modelIdx = databaseModel->index(i, column);
		if(databaseModel->data(modelIdx).toString().contains(ui->searchTextEdit->text())) {
			ui->databaseTable->selectionModel()->select(modelIdx, QItemSelectionModel::SelectCurrent);
			ui->databaseTable->scrollTo(modelIdx, QAbstractItemView::PositionAtCenter);
			found = true;
			break;
		}
	}
	if(!found) {
		ui->searchTextEdit->setStyleSheet("background:#ff0000");
		searchNotFoundStyleTimer.start();
	}
}

void DatabaseView::onSearchResetStyle() {
	ui->searchTextEdit->setStyleSheet("");
}

void DatabaseView::onChangeLocale(int newIndex) {
	databaseModel->changeLocale(ui->localeCombo->itemData(newIndex).toByteArray());
}

void DatabaseView::onModifyDb(QModelIndex, QModelIndex) {
	savedData = false;
	setWindowTitle(loadedDatabaseName);
}

void DatabaseView::setStatus(eToolStatus newStatus) {
	QString newMessage;

	currentStatus = newStatus;

	switch(currentStatus) {
		case TS_NoDbDescLoaded: newMessage = tr("No Database Description Loaded", "Status in statusbar");   break;
		case TS_LoadingDbDesc:  newMessage = tr("Loading Database Description ...", "Status in statusbar"); break;
		case TS_NoDbLoaded:     newMessage = tr("No Database Loaded", "Status in statusbar");               break;
		case TS_LoadingDB:      newMessage = tr("Loading Database ...", "Status in statusbar");             break;
		case TS_DbLoaded:       newMessage = tr("Database loaded, %1 rows", "Status in statusbar").arg(db ? db->getRowCount() : 0);                  break;
		case TS_SavingDB:       newMessage = tr("Saving Database ...", "Status in statusbar");              break;
		case TS_ClosingDB:      newMessage = tr("Closing Database ...", "Status in statusbar");             break;
	}

	if(db && db->getDate() && currentStatus == TS_DbLoaded) {
		newMessage += " | " + tr("Creation date: %1").arg(QDateTime::fromTime_t(db->getDate()).toString(tr("yyyy/MM/dd", "RDB Creation date format shown in statusbar")));
	}

	if(!newMessage.isNull())
		statusBarLabel->setText(newMessage);
}

void DatabaseView::setWindowTitle(const QString &title) {
	QString shortTitle = QFileInfo(title).fileName();
	if(savedData == false)
		QWidget::setWindowTitle(shortTitle + "*");
	else
		QWidget::setWindowTitle(shortTitle);

	emit titleChanged(this);
}

QString DatabaseView::errorToString(eDataSourceType type, int error, bool save) {
	switch(error) {
		case ENOENT:
			if(IS_DST_FILESOURCE(type))
				return tr("The file does not exist", "Load/Save file error messagebox");
			else if(!save)
				return tr("The SQL table or database does not exist", "Load SQL table error messagebox");
			else
				return tr("The database does not exist", "Save SQL table error messagebox");

			break;

		case ENXIO:
			return tr("Can\'t connect to data source, check SQL configuration and ODBC data sources", "Load/Save SQL table error messagebox");

		case EIO:
			if(save)
				return tr("Can\'t save the file, I/O error", "Load/Save file error messagebox");
			else
				return tr("Can\'t load the file, I/O error", "Load/Save file error messagebox, not related to the existence of the file");

		case EINVAL:
			return tr("Can\'t read the file, invalid format", "Load/Save file error messagebox, the file has not the expected file format");

		case ENOMEM:
			return tr("No enough memory", "Load/Save error messagebox, memory allocation failed, will probably cause a crash later");

		case ENOSYS:
			return tr("Operation not supported", "Load/Save error messagebox, non implemented/unsuppored operation");

		case ENOEXEC:
			return tr("Error while executing a SQL query, aborting", "Load/Save SQL table error messagebox");

		case EILSEQ:
			return tr("Error while executing a INSERT sql query, aborting", "Save SQL table error messagebox");

		default:
			return tr("Unknown error: %1", "Load/Save error messagebox, unknown error, %1 is a generated message from the error").arg(strerror(error));
	}
}
