#ifndef SQLCONFIGDIALOG_H
#define SQLCONFIGDIALOG_H

#include <QDialog>
#include "../Base/DataType.h"

namespace Ui {
class SqlConfigDialog;
}

class QSettings;

class SqlConfigDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit SqlConfigDialog(QSettings *settings);
		~SqlConfigDialog();

		eDataSourceType getServerType();
		QString getServerName();
		QString getUsername();
		QString getPassword();

	protected slots:
		void onConfigureOdbc();
		
	private:
		Ui::SqlConfigDialog *ui;
		QSettings *settings;
};

#endif // SQLCONFIGDIALOG_H
