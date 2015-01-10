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

		QString getConnectionString(int tabIndex = -1);

		bool isConfigSet();

	protected slots:
		void onTabChanged(int newIndex);

	private:
		QString getServerIp();
		int getServerPort();
		QString getUsername();
		QString getPassword();

		Ui::SqlConfigDialog *ui;
};

#endif // SQLCONFIGDIALOG_H
