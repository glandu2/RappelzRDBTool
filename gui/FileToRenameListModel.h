#ifndef FILETORENAMELISTMODEL_H
#define FILETORENAMELISTMODEL_H

#include <QAbstractTableModel>

class FileToRenameListModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	FileToRenameListModel(QObject *parent = 0);
	~FileToRenameListModel();

	void append(QString filename);
	void remove(int index);
	QString getSourceFilename(int index);
	QString getTargetFilename(int index);
	int getCount() { return fileList.size(); }

	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

protected:
	struct FileInfo {
		QString filename;
		bool isHashed;
	};

	static QString convertFilename(FileInfo fileInfo);
	static QByteArray convertHashToName(QByteArray hash);
	static QByteArray convertNameToHash(QByteArray name);
	static bool isHashedName(QString filename);

private:

	QList<FileInfo> fileList;
	QString optionGroupName;

};

#endif // FILETORENAMELISTMODEL_H
