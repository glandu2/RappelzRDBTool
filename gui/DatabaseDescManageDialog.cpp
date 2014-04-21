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
	QStringList fileToAdd = QFileDialog::getOpenFileNames(this, QCoreApplication::applicationName(), QString(), tr("Database DLL (*Database.dll *Database.so)", "File dialogbox filter for database description DLL"));

	for(int i = 0; i < fileToAdd.size(); i++) {
		const QString& fileName = fileToAdd.at(i);
		const QByteArray fileName8bits = fileName.toLocal8Bit();
		int result, sysError;

		IDatabaseDescription* dbDesc = createExternDescriptedDatabase();

		result = dbDesc->open(fileName8bits.constData(), &sysError);
		if(result == 0) {
			dbDescriptionModel->append(dbDesc);
		} else {
			dbDesc->destroy();

			//Use fromLocal8Bit(toLocal8Bit()) to have the true used filename
			switch(result) {
				case ENOENT:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 tr("Can\'t load %1, are you sure the file exist and has the same bits (32 or 64) as this gui ?\nAdditional error code: %2", "Error while loading a database description DLL")
										 .arg(QString::fromLocal8Bit(fileName8bits.constData()))
										 .arg(sysError));
					break;

				case EINVAL:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 tr("Can\'t use %1, this is not a database description file", "Error while loading a database description DLL")
										 .arg(QString::fromLocal8Bit(fileName8bits.constData())));
					break;

				default:
					QMessageBox::warning(this, QCoreApplication::applicationName(),
										 tr("Unknown error while loading %1.\nAdditional error code: %2", "Error while loading a database description DLL")
										 .arg(QString::fromLocal8Bit(fileName8bits.constData()))
										 .arg(sysError));
					break;
			}
		}
	}
}

static bool compareIndexAsc(const QModelIndex& i1, const QModelIndex& i2) {
	//i1 is before i2 if it has a greater row index
	return i1.row() > i2.row();
}

void DatabaseDescManageDialog::onRemove() {
	QModelIndexList indices = ui->dbDescTable->selectionModel()->selectedRows();
	QModelIndex index;

	//We must remove latest entries before first one (to keep indices valid)
	qSort(indices.begin(), indices.end(), &compareIndexAsc);
	foreach(index, indices) {
		if(index.isValid())
			dbDescriptionModel->remove(index.row());
	}
}
