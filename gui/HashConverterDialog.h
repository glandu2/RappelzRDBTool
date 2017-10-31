#ifndef HASHCONVERTERDIALOG_H
#define HASHCONVERTERDIALOG_H

#include <QDialog>

namespace Ui {
class HashConverterDialog;
}

class FileToRenameListModel;

class HashConverterDialog : public QDialog {
	Q_OBJECT

public:
	explicit HashConverterDialog(QWidget* parent = 0);
	~HashConverterDialog();

public slots:
	void updateName();
	void updateHash();

	void addFile();
	void removeFile();
	void renameFiles();

private:
	Ui::HashConverterDialog* ui;
	FileToRenameListModel* fileListmodel;
};

#endif  // HASHCONVERTERDIALOG_H
