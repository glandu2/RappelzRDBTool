#include "SqlConfigDialog.h"
#include "ui_SqlConfigDialog.h"

#include "Settings.h"
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

#ifdef WIN32
#include <windows.h>
#endif

SqlConfigDialog::SqlConfigDialog() :
	QDialog(0),
	ui(new Ui::SqlConfigDialog)
{
	ui->setupUi(this);
#ifdef WIN32
	connect(ui->configureOdbcButton, SIGNAL(clicked()), this, SLOT(onConfigureOdbc()));
#else
	ui->verticalLayout->removeWidget(ui->configureOdbcButton);
	ui->configureOdbcButton->deleteLater();
#endif

	ui->serverTypeCombo->setCurrentIndex(Settings::getSettings()->value("SqlConfig/serverType").toInt());
	ui->serverNameEdit->setText(Settings::getSettings()->value("SqlConfig/serverName").toString());

	ui->usernameEdit->setText(Settings::getSettings()->value("SqlConfig/username").toString());
	ui->passwordSaveCheck->setChecked(Settings::getSettings()->value("SqlConfig/savePassword", false).toBool());
	if(ui->passwordSaveCheck->isChecked())
		ui->passwordEdit->setText(Settings::getSettings()->value("SqlConfig/password").toString());
}

SqlConfigDialog::~SqlConfigDialog()
{
	Settings::getSettings()->setValue("SqlConfig/serverType", ui->serverTypeCombo->currentIndex());
	Settings::getSettings()->setValue("SqlConfig/serverName", ui->serverNameEdit->text());
	Settings::getSettings()->setValue("SqlConfig/username", ui->usernameEdit->text());
	Settings::getSettings()->setValue("SqlConfig/savePassword", ui->passwordSaveCheck->isChecked());

	if(ui->passwordSaveCheck->isChecked())
		Settings::getSettings()->setValue("SqlConfig/password", ui->passwordEdit->text());
	else Settings::getSettings()->remove("SqlConfig/password");
	delete ui;
}

void SqlConfigDialog::onConfigureOdbc() {
#ifdef WIN32
	ShellExecute(0, L"runas", L"odbcad32.exe", 0, 0, SW_SHOWNORMAL);
#elif defined(__unix__)
	QDesktopServices::openUrl(QUrl(QDir::homePath() + "/.odbc.ini"));
#endif
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
	return ui->serverNameEdit->text();
}

QString SqlConfigDialog::getUsername() {
	return ui->usernameEdit->text();
}

QString SqlConfigDialog::getPassword() {
	return ui->passwordEdit->text();
}
