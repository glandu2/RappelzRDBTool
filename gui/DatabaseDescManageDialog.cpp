#include "DatabaseDescManageDialog.h"
#include "ui_DatabaseDescManageDialog.h"

#include "DatabaseDescriptionListModel.h"
#include "../Base/IDatabaseDescription.h"

#include <QFileDialog>

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
		IDatabaseDescription* dbDesc = createExternDescriptedDatabase(fileName.toLatin1().constData());
		if(dbDesc) {
			dbDescriptionModel->append(dbDesc);
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
