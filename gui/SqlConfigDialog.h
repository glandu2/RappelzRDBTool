#ifndef SQLCONFIGDIALOG_H
#define SQLCONFIGDIALOG_H

#include <QDialog>
#include "../Base/DataType.h"

namespace Ui {
class SqlConfigDialog;
}

class SqlConfigDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit SqlConfigDialog();
		~SqlConfigDialog();

		eDataSourceType getServerType();
		QString getServerIp();
		int getServerPort();
		QString getUsername();
		QString getPassword();
		
	private:
		Ui::SqlConfigDialog *ui;
};

#endif // SQLCONFIGDIALOG_H
