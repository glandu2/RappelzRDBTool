#include "SqlConfigDialog.h"
#include "ui_SqlConfigDialog.h"

#include "Settings.h"
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

#ifdef __unix__
#  include <sql.h>
#else
#  undef UNICODE
#  include <windows.h>
#endif
#include <sqlext.h>

#ifdef WIN32
#include <windows.h>
#endif

SqlConfigDialog::SqlConfigDialog() :
	QDialog(0),
	ui(new Ui::SqlConfigDialog)
{
	ui->setupUi(this);

	connect(ui->configureOdbcButton, SIGNAL(clicked()), this, SLOT(onConfigureOdbc()));

	ui->serverTypeCombo->setCurrentIndex(Settings::getSettings()->value("SqlConfig/serverType").toInt());
	ui->serverNameCombo->setEditText(Settings::getSettings()->value("SqlConfig/serverName").toString());

	ui->usernameEdit->setText(Settings::getSettings()->value("SqlConfig/username").toString());
	ui->passwordSaveCheck->setChecked(Settings::getSettings()->value("SqlConfig/savePassword", false).toBool());
	if(ui->passwordSaveCheck->isChecked())
		ui->passwordEdit->setText(Settings::getSettings()->value("SqlConfig/password").toString());

	updateDsnList();
}

SqlConfigDialog::~SqlConfigDialog()
{
	Settings::getSettings()->setValue("SqlConfig/serverType", ui->serverTypeCombo->currentIndex());
	Settings::getSettings()->setValue("SqlConfig/serverName", ui->serverNameCombo->currentText());
	Settings::getSettings()->setValue("SqlConfig/username", ui->usernameEdit->text());
	Settings::getSettings()->setValue("SqlConfig/savePassword", ui->passwordSaveCheck->isChecked());

	if(ui->passwordSaveCheck->isChecked())
		Settings::getSettings()->setValue("SqlConfig/password", ui->passwordEdit->text());
	else Settings::getSettings()->remove("SqlConfig/password");
	delete ui;
}

void SqlConfigDialog::onConfigureOdbc() {
#ifdef WIN32
	ShellExecute(0, "runas", "odbcad32.exe", 0, 0, SW_SHOWNORMAL);
#elif defined(__unix__)
	QDesktopServices::openUrl(QUrl(QDir::homePath() + "/.odbc.ini"));
#endif
	updateDsnList();
}

static void printOdbcStatus(SQLSMALLINT type, HSTMT hstmt) {
	SQLCHAR     buffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLCHAR     sqlstate[SQL_SQLSTATE_SIZE + 1];
	SQLINTEGER  sqlcode;
	SQLSMALLINT length;
	int i=1;
	while ( SQLGetDiagRec( type, hstmt, i, sqlstate, &sqlcode, buffer, SQL_MAX_MESSAGE_LENGTH + 1, &length) == SQL_SUCCESS ) {
		printf("SQLSTATE: %s\n", sqlstate);
		printf("Native Error Code: %d\n", sqlcode);
		printf("buffer: %s \n\n", buffer);
		i++;
	}
}

void SqlConfigDialog::updateDsnList() {
	HENV henv;
	char dsn[128] = {0};
	char desc[128] = {0};
	SQLSMALLINT dummy;
	int result;

	result = SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);
	if(!SQL_SUCCEEDED(result))
		return;

	result = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	if(!SQL_SUCCEEDED(result)) {
		SQLFreeHandle(SQL_HANDLE_ENV, henv);
		return;
	}

	QString oldtext = ui->serverNameCombo->currentText();

	ui->serverNameCombo->clear();
	result = SQLDataSources(henv, SQL_FETCH_FIRST, (SQLCHAR*)dsn, 128, &dummy, (SQLCHAR*)desc, 128, &dummy);
	while(SQL_SUCCEEDED(result)) {
		ui->serverNameCombo->addItem(QString(dsn) + " - " + QString(desc));
		memset(dsn, 0, 128);
		memset(desc, 0, 128);
		result = SQLDataSources(henv, SQL_FETCH_NEXT, (SQLCHAR*)dsn, 128, &dummy, (SQLCHAR*)desc, 128, &dummy);
	}

	if(!SQL_SUCCEEDED(result))
		printOdbcStatus(SQL_HANDLE_ENV, henv);

	ui->serverNameCombo->setEditText(oldtext);

	SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

eDataSourceType SqlConfigDialog::getServerType() {
	if(ui->serverTypeCombo->currentIndex() == 0)
		return DST_SQLServer;
	else if(ui->serverTypeCombo->currentIndex() == 1)
		return DST_SQLPostgres;
	else {
		ui->serverTypeCombo->setCurrentIndex(0);
		return DST_SQLServer;
	}
}

QString SqlConfigDialog::getServerName() {
	int index = ui->serverNameCombo->currentText().indexOf(' ');

	return ui->serverNameCombo->currentText().mid(0, index);
}

QString SqlConfigDialog::getUsername() {
	return ui->usernameEdit->text();
}

QString SqlConfigDialog::getPassword() {
	return ui->passwordEdit->text();
}
