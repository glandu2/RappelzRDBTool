#include "HashConverterDialog.h"
#include "ui_HashConverterDialog.h"
#include "NameToHash.h"
#include "HashToName.h"
#include <QFileDialog>
#include "FileToRenameListModel.h"

HashConverterDialog::HashConverterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HashConverterDialog)
{
	ui->setupUi(this);

	fileListmodel = new FileToRenameListModel(this);
	ui->filesToRenameTable->setModel(fileListmodel);

	connect(ui->hashToNameButton, SIGNAL(clicked()), this, SLOT(updateName()));
	connect(ui->nameToHashButton, SIGNAL(clicked()), this, SLOT(updateHash()));

	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addFile()));
	connect(ui->removeButton, SIGNAL(clicked()), this, SLOT(removeFile()));
	connect(ui->renameButton, SIGNAL(clicked()), this, SLOT(renameFiles()));
}

HashConverterDialog::~HashConverterDialog()
{
	delete ui;
}

void HashConverterDialog::updateName() {
	QByteArray hash = ui->hashEdit->text().toLatin1();
	QByteArray name;

	if(hash.size() >= 2) {
		name.resize(hash.size()-1);
		convertHashToName(hash.constData(), name.data());
		name.resize(name.size()-1);

		ui->nameEdit->setText(name);
	}
}

void HashConverterDialog::updateHash() {
	QByteArray name = ui->nameEdit->text().toLatin1();
	QByteArray hash;

	hash.resize(name.size()+3);
	convertNameToHash(name.constData(), hash.data(), LEGACY_SEED);
	hash.resize(hash.size()-1);

	ui->hashEdit->setText(hash);
}

void HashConverterDialog::addFile() {
	QStringList fileToAdd = QFileDialog::getOpenFileNames(this, QCoreApplication::applicationName(), QString(), tr("RDB files (*.rdb);;All files (*)", "File dialogbox filter for hash renamer"));

	for(int i = 0; i < fileToAdd.size(); i++) {
		fileListmodel->append(fileToAdd.at(i));
	}
}

static bool compareIndexAsc(const QModelIndex& i1, const QModelIndex& i2) {
	//i1 is before i2 if it has a greater row index
	return i1.row() > i2.row();
}

void HashConverterDialog::removeFile() {
	QModelIndexList indices = ui->filesToRenameTable->selectionModel()->selectedRows();
	QModelIndex index;

	//We must remove latest entries before first one (to keep indices valid)
	qSort(indices.begin(), indices.end(), &compareIndexAsc);
	foreach(index, indices) {
		if(index.isValid())
			fileListmodel->remove(index.row());
	}
}

void HashConverterDialog::renameFiles() {
	int count = fileListmodel->getCount();

	for(int i = 0; i < count; i++) {
		QFile file(fileListmodel->getSourceFilename(i));
		if(file.exists()) {
			QFile::remove(fileListmodel->getTargetFilename(i));
			file.copy(fileListmodel->getTargetFilename(i));
		}
	}
}
