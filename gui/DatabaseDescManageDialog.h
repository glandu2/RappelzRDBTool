#ifndef DATABASEDESCMANAGEDIALOG_H
#define DATABASEDESCMANAGEDIALOG_H

#include <QDialog>

namespace Ui {
class DatabaseDescManageDialog;
}

class DatabaseDescriptionListModel;

class DatabaseDescManageDialog : public QDialog
{
		Q_OBJECT
		
	public:
		DatabaseDescManageDialog(DatabaseDescriptionListModel* dbDescListModel, QWidget *parent = 0);
		~DatabaseDescManageDialog();

	public slots:
		void onAdd();
		void onRemove();
		
	private:
		Ui::DatabaseDescManageDialog *ui;
		DatabaseDescriptionListModel *dbDescriptionModel;
};

#endif // DATABASEDESCMANAGEDIALOG_H
