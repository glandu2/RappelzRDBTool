#include "SqlConfigDialog.h"
#include "ui_SqlConfigDialog.h"

#include <QSettings>

#ifdef WIN32
#include <windows.h>
#endif

SqlConfigDialog::SqlConfigDialog(QSettings *settings) :
	QDialog(0),
	ui(new Ui::SqlConfigDialog),
	settings(settings)
{
	ui->setupUi(this);
#ifdef WIN32
	connect(ui->configureOdbcButton, SIGNAL(clicked()), this, SLOT(onConfigureOdbc()));
#else
	ui->verticalLayout->removeWidget(ui->configureOdbcButton);
	ui->configureOdbcButton->deleteLater();
#endif

	ui->serverTypeCombo->setCurrentIndex(settings->value("SqlConfig/serverType").toInt());
	ui->serverNameEdit->setText(settings->value("SqlConfig/serverName").toString());

	ui->usernameEdit->setText(settings->value("SqlConfig/username").toString());
	ui->passwordSaveCheck->setChecked(settings->value("SqlConfig/savePassword", false).toBool());
	if(ui->passwordSaveCheck->isChecked())
		ui->passwordEdit->setText(settings->value("SqlConfig/password").toString());
}

SqlConfigDialog::~SqlConfigDialog()
{
	settings->setValue("SqlConfig/serverType", ui->serverTypeCombo->currentIndex());
	settings->setValue("SqlConfig/serverName", ui->serverNameEdit->text());
	settings->setValue("SqlConfig/username", ui->usernameEdit->text());
	settings->setValue("SqlConfig/savePassword", ui->passwordSaveCheck->isChecked());

	if(ui->passwordSaveCheck->isChecked())
		settings->setValue("SqlConfig/password", ui->passwordEdit->text());
	else settings->remove("SqlConfig/password");
	delete ui;
}

void SqlConfigDialog::onConfigureOdbc() {
#ifdef WIN32
	ShellExecute(0, L"runas", L"odbcad32.exe", 0, 0, SW_SHOWNORMAL);
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
