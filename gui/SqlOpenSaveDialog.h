#ifndef SQLOPENSAVEDIALOG_H
#define SQLOPENSAVEDIALOG_H

#include <QDialog>

namespace Ui {
class SqlOpenSaveDialog;
}

class SqlOpenSaveDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SqlOpenSaveDialog(QWidget *parent = 0);
	~SqlOpenSaveDialog();

	void setMessage(const QString& message);
	void setTableName(const QString& message);
	QString getTableName();
	void setUseExistingTableSchemaEnabled(bool enable);
	bool useExistingTableSchema();

private:
	Ui::SqlOpenSaveDialog *ui;
};

#endif // SQLOPENSAVEDIALOG_H
