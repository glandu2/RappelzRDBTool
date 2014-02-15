#include "HashConverterDialog.h"
#include "ui_HashConverterDialog.h"
#include "NameToHash.h"
#include "HashToName.h"

HashConverterDialog::HashConverterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HashConverterDialog)
{
	ui->setupUi(this);
	connect(ui->hashToNameButton, SIGNAL(clicked()), this, SLOT(updateName()));
	connect(ui->nameToHashButton, SIGNAL(clicked()), this, SLOT(updateHash()));
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
