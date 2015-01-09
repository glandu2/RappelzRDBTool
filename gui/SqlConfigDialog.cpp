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

	ui->serverTypeCombo->setCurrentIndex(Settings::getSettings()->value("SqlConfig/serverType").toInt());
	ui->serverIpEdit->setText(Settings::getSettings()->value("SqlConfig/serverIp").toString());
	ui->serverPortEdit->setText(Settings::getSettings()->value("SqlConfig/serverPort").toString());

	ui->usernameEdit->setText(Settings::getSettings()->value("SqlConfig/username").toString());
	ui->passwordSaveCheck->setChecked(Settings::getSettings()->value("SqlConfig/savePassword", false).toBool());
	if(ui->passwordSaveCheck->isChecked())
		ui->passwordEdit->setText(Settings::getSettings()->value("SqlConfig/password").toString());
}

SqlConfigDialog::~SqlConfigDialog()
{
	Settings::getSettings()->setValue("SqlConfig/serverType", ui->serverTypeCombo->currentIndex());
	Settings::getSettings()->setValue("SqlConfig/serverIp", ui->serverIpEdit->text());
	Settings::getSettings()->setValue("SqlConfig/serverPort", ui->serverPortEdit->text());
	Settings::getSettings()->setValue("SqlConfig/username", ui->usernameEdit->text());
	Settings::getSettings()->setValue("SqlConfig/savePassword", ui->passwordSaveCheck->isChecked());

	if(ui->passwordSaveCheck->isChecked())
		Settings::getSettings()->setValue("SqlConfig/password", ui->passwordEdit->text());
	else Settings::getSettings()->remove("SqlConfig/password");
	delete ui;
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
