#include "DatabaseDescManageDialog.h"
#include "ui_DatabaseDescManageDialog.h"

#include "DatabaseDescriptionListModel.h"
#include "../Base/IDatabaseDescription.h"

#include <QFileDialog>
#include <QMessageBox>
#include <errno.h>

DatabaseDescManageDialog::DatabaseDescManageDialog(DatabaseDescriptionListModel *dbDescListModel, QWidget *parent) :
    QDialog(parent),
	ui(new Ui::DatabaseDescManageDialog),
	dbDescriptionModel(dbDescListModel)
{
	ui->setupUi(this);
	setWindowFlags(windowFlags() | Qt::WindowMinMaxButtonsHint);
	ui->dbDescTable->setModel(dbDescriptionModel);

	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(onAdd()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(onRemove()));
	connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));

	ui->dbDescTable->resizeColumnsToContents();
}

DatabaseDescManageDialog::~DatabaseDescManageDialog()
{
	delete ui;
}


void DatabaseDescManageDialog::onAdd() {
	QStringList fileToAdd = QFileDialog::getOpenFileNames(this, QCoreApplication::applicationName(), QString(), "Database DLL (*Database.dll *Database.so)");

	for(int i = 0; i < fileToAdd.size(); i++) {
		const QString& fileName = fileToAdd.at(i);
		int result, sysError;

		IDatabaseDescription* dbDesc = createExternDescriptedDatabase();

		result = dbDesc->open(fileName.toLocal8Bit().constData(), &sysError);
		if(result == 0) {
			dbDescriptionModel->append(dbDesc);
		} else {
			dbDesc->destroy();
			switch(result) {
				case ENOENT:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 QString("Can\'t load %1, are you sure the file exist and has the same bits (32 or 64) as this gui ?\nAdditional error code: %2").arg(QString::fromLocal8Bit(fileName.toLocal8Bit().constData())).arg(sysError));
					break;

				case EINVAL:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 QString("Can\'t use %1, this is not a database description file").arg(QString::fromLocal8Bit(fileName.toLocal8Bit().constData())));
					break;

				default:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 QString("Unknown error while loading %1.\nAdditional error code: %2").arg(QString::fromLocal8Bit(fileName.toLocal8Bit().constData()), sysError));
					break;
			}
		}
	}
}

void DatabaseDescManageDialog::onRemove() {
	QModelIndexList indices = ui->dbDescTable->selectionModel()->selectedRows();
	QModelIndex index;

	foreach(index, indices) {
		if(index.isValid())
			dbDescriptionModel->remove(index.row());
	}
}
