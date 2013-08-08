#ifndef DATABASEDESCRIPTIONLISTMODEL_H
#define DATABASEDESCRIPTIONLISTMODEL_H

#include <QAbstractListModel>
#include <QList>

class IDatabaseDescription;
class QSettings;

class DatabaseDescriptionListModel : public QAbstractTableModel
{
	public:
		DatabaseDescriptionListModel(QSettings *settings, QObject *parent = 0);
		~DatabaseDescriptionListModel();

		void append(IDatabaseDescription* value);
		void remove(int index);

		IDatabaseDescription* getDbDescription(int index) { return dbDescriptions.at(index); }

		virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
		virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
		virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

	private:
		QList<IDatabaseDescription*> dbDescriptions;
		QSettings *options;
		QString optionGroupName;

};

#endif // DATABASEDESCRIPTIONLISTMODEL_H
