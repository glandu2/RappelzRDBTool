#include "DatabaseDescriptionListModel.h"
#include "../Base/IDatabaseDescription.h"

#include <QSettings>
#include <QDir>
#include <QFileInfoList>

DatabaseDescriptionListModel::DatabaseDescriptionListModel(QSettings *settings, QObject *parent) :
	QAbstractTableModel(parent),
	options(settings)
{
	int i = 0;
	QString value;

#ifdef __linux__
	optionGroupName = "DbStructsLinux";
#else
	optionGroupName = "DbStructsWindows";
#endif

	while((value = options->value((optionGroupName + "/filepath/%1").arg(i)).toString()).isNull() == false) {
		IDatabaseDescription* dbDesc = createExternDescriptedDatabase(value.toLatin1().constData());
		if(dbDesc) {
			dbDescriptions.append(dbDesc);
		}
		i++;
	}

	QDir currentDir;
	QFileInfoList subFiles = currentDir.entryInfoList(QDir::Files, QDir::Name);
	for(i = 0; i < subFiles.size(); i++) {
		const QFileInfo& fileInfo = subFiles.at(i);
#ifdef __linux__
		if(fileInfo.fileName().endsWith("Database.so"))
#else
		if(fileInfo.fileName().endsWith("Database.dll"))
#endif
		{
			IDatabaseDescription* dbDesc = createExternDescriptedDatabase(fileInfo.absoluteFilePath().toLatin1().constData());
			if(dbDesc) {
				append(dbDesc);
			}
		}
	}
}

DatabaseDescriptionListModel::~DatabaseDescriptionListModel() {
	options->remove(optionGroupName + "/filepath");
	for(int i = 0; i < dbDescriptions.size(); i++)
		options->setValue((optionGroupName + "/filepath/%1").arg(i), QString(dbDescriptions.at(i)->getFilename()));
}

void DatabaseDescriptionListModel::append(IDatabaseDescription* value) {
	IDatabaseDescription *currentDbDesc;

	foreach(currentDbDesc, dbDescriptions) {
		if(!qstrcmp(currentDbDesc->getFilename(), value->getFilename()))
			return;
	}

	beginInsertRows(QModelIndex(), dbDescriptions.size(), dbDescriptions.size());
	dbDescriptions.append(value);
	endInsertRows();
}

void DatabaseDescriptionListModel::remove(int index) {
	if(index != -1) {
		beginRemoveRows(QModelIndex(), index, index);
		dbDescriptions.removeAt(index);
		endRemoveRows();
	}
}

int DatabaseDescriptionListModel::columnCount(const QModelIndex& parent) const {
	if(parent.isValid())
		return 0;
	return 2;
}

QVariant DatabaseDescriptionListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();

	if(section == 0)
		return QVariant("Name");
	else if(section == 1)
		return QVariant("File name");

	return QVariant();
}

int DatabaseDescriptionListModel::rowCount(const QModelIndex& parent) const {
	if(parent.isValid())
		return 0;
	return dbDescriptions.size();
}

QVariant DatabaseDescriptionListModel::data(const QModelIndex& index, int role) const {
	if(!index.isValid() || index.parent().isValid() || role != Qt::DisplayRole)
		return QVariant();

	if(index.column() == 0)
		return QFileInfo(QString(dbDescriptions.at(index.row())->getFilename())).fileName();
	else if(index.column() == 1)
		return QString(dbDescriptions.at(index.row())->getFilename());

	return QVariant();
}
