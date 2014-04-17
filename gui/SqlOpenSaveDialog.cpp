#include "SqlOpenSaveDialog.h"
#include "ui_SqlOpenSaveDialog.h"

SqlOpenSaveDialog::SqlOpenSaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SqlOpenSaveDialog)
{
	ui->setupUi(this);
	setWindowTitle(QApplication::applicationName());
	layout()->setSizeConstraint(QLayout::SetFixedSize);
}

SqlOpenSaveDialog::~SqlOpenSaveDialog()
{
	delete ui;
}

void SqlOpenSaveDialog::setMessage(const QString& message) {
	ui->messageLabel->setText(message);
}

void SqlOpenSaveDialog::setTableName(const QString& message) {
	ui->tableNameEdit->setText(message);
}

QString SqlOpenSaveDialog::getTableName() {
	return ui->tableNameEdit->text();
}

void SqlOpenSaveDialog::setUseExistingTableSchemaEnabled(bool enable) {
	ui->useExistingTableCheckbox->setShown(enable);
}

bool SqlOpenSaveDialog::useExistingTableSchema() {
	return ui->useExistingTableCheckbox->isChecked();
}
