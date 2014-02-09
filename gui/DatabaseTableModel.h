#ifndef DATABASETABLEMODEL_H
#define DATABASETABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

class IDatabase;
class IRowManipulator;
class QTextCodec;

class DatabaseTableModel : public QAbstractTableModel
{
		Q_OBJECT
	public:
		explicit DatabaseTableModel(QObject *parent = 0);

		void bindToDatabase(IDatabase *db);
		void unbindDatabase();

		virtual Qt::ItemFlags flags(const QModelIndex& index) const;

		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

		virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);

	public slots:
		void onChangeLocale(QString newLocale);

	private:
		IDatabase *db;
		IRowManipulator *row;
		QVector<int> columnBinding;
		QTextCodec *currentLocale;
		
};

#endif // DATABASETABLEMODEL_H
