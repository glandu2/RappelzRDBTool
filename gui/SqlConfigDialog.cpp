#include "SqlConfigDialog.h"
#include "ui_SqlConfigDialog.h"

#include "Settings.h"
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

#ifdef __unix__
#include <sql.h>
#else
#undef UNICODE
#include <windows.h>
#endif
#include <sqlext.h>

#ifdef WIN32
#include <windows.h>
#endif

SqlConfigDialog::SqlConfigDialog() : QDialog(0), ui(new Ui::SqlConfigDialog) {
	ui->setupUi(this);

	ui->serverTypeCombo->setCurrentIndex(Settings::getSettings()->value("SqlConfig/serverType").toInt());
	ui->connectionStringEdit->setText(Settings::getSettings()->value("SqlConfig/connectionString").toString());
	ui->serverIpEdit->setText(Settings::getSettings()->value("SqlConfig/serverIp").toString());
	ui->serverPortEdit->setText(Settings::getSettings()->value("SqlConfig/serverPort").toString());

	ui->usernameEdit->setText(Settings::getSettings()->value("SqlConfig/username").toString());
	ui->passwordSaveCheck->setChecked(Settings::getSettings()->value("SqlConfig/savePassword", false).toBool());
	if(ui->passwordSaveCheck->isChecked())
		ui->passwordEdit->setText(Settings::getSettings()->value("SqlConfig/password").toString());

	resize(width(), sizeHint().height());
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
}

SqlConfigDialog::~SqlConfigDialog() {
	Settings::getSettings()->setValue("SqlConfig/serverType", ui->serverTypeCombo->currentIndex());
	Settings::getSettings()->setValue("SqlConfig/connectionString", ui->connectionStringEdit->text());
	Settings::getSettings()->setValue("SqlConfig/serverIp", ui->serverIpEdit->text());
	Settings::getSettings()->setValue("SqlConfig/serverPort", ui->serverPortEdit->text());
	Settings::getSettings()->setValue("SqlConfig/username", ui->usernameEdit->text());
	Settings::getSettings()->setValue("SqlConfig/savePassword", ui->passwordSaveCheck->isChecked());

	if(ui->passwordSaveCheck->isChecked())
		Settings::getSettings()->setValue("SqlConfig/password", ui->passwordEdit->text());
	else
		Settings::getSettings()->remove("SqlConfig/password");
	delete ui;
}

void SqlConfigDialog::onTabChanged(int newIndex) {
	if(newIndex == 1 && ui->connectionStringEdit->text().isEmpty())
		ui->connectionStringEdit->setText(getConnectionString(0));
}

bool SqlConfigDialog::isConfigSet() {
	if(ui->tabWidget->currentIndex() == 0)
		return !ui->serverIpEdit->text().isEmpty();
	else
		return !ui->connectionStringEdit->text().isEmpty();
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

QString SqlConfigDialog::getConnectionString(int tabIndex) {
	if(tabIndex == -1)
		tabIndex = ui->tabWidget->currentIndex();

	if(tabIndex == 0) {
		QString connectionString;
		int sqlPort = getServerPort();

		if(getServerType() == DST_SQLServer) {
			if(!sqlPort)
				sqlPort = 1433;
#ifdef _WIN32
			connectionString = "Driver=SQL Server;Server=%0,%1;UID=%2;PWD=%3;";
#else
			connectionString = "Driver=FreeTDS;TDS_Version=7.2;Server=%0,%1;UID=%2;PWD=%3;";
#endif
		} else if(getServerType() == DST_SQLPostgres) {
			if(!sqlPort)
				sqlPort = 5432;
#ifdef _WIN64
			connectionString = "Driver={PostgreSQL Unicode(x64)};Server=%0;Port=%1;UID=%2;PWD=%3;";
#else
			connectionString = "Driver={PostgreSQL Unicode};Server=%0;Port=%1;UID=%2;PWD=%3;";
#endif
		}

		return connectionString.arg(getServerIp(), QString::number(sqlPort), getUsername(), getPassword());
	} else {
		return ui->connectionStringEdit->text();
	}
}

QString SqlConfigDialog::getServerIp() {
	return ui->serverIpEdit->text();
}

int SqlConfigDialog::getServerPort() {
	return ui->serverPortEdit->text().toInt();
}

QString SqlConfigDialog::getUsername() {
	return ui->usernameEdit->text();
}

QString SqlConfigDialog::getPassword() {
	return ui->passwordEdit->text();
}
