#ifndef HASHCONVERTERDIALOG_H
#define HASHCONVERTERDIALOG_H

#include <QDialog>

namespace Ui {
class HashConverterDialog;
}

class HashConverterDialog : public QDialog
{
	Q_OBJECT

public:
	explicit HashConverterDialog(QWidget *parent = 0);
	~HashConverterDialog();

public slots:
	void updateName();
	void updateHash();

private:
	Ui::HashConverterDialog *ui;
};

#endif // HASHCONVERTERDIALOG_H
