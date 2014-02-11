#include "DatabaseDescriptionListModel.h"
#include "../Base/IDatabaseDescription.h"
#include "NameToHash.h"
#include "Settings.h"

#include <QSettings>
#include <QDir>
#include <QFileInfoList>

bool compareDbDescriptionNameLessThan(IDatabaseDescription* a, IDatabaseDescription* b) {
	return strcmp(a->getFilename(), b->getFilename()) < 0;
}

DatabaseDescriptionListModel::DatabaseDescriptionListModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	int i = 0;
	QString value;

#ifdef __linux__
	optionGroupName = "DbStructsLinux";
#else
	optionGroupName = "DbStructsWindows";
#endif

	while((value = Settings::getSettings()->value((optionGroupName + "/filepath/%1").arg(i)).toString()).isNull() == false) {
		IDatabaseDescription* dbDesc = createExternDescriptedDatabase();
		if(dbDesc->open(value.toLocal8Bit().constData(), 0) == 0) {
			dbDescriptions.append(dbDesc);
		} else {
			dbDesc->destroy();
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
			IDatabaseDescription* dbDesc = createExternDescriptedDatabase();
			if(dbDesc->open(fileInfo.absoluteFilePath().toLocal8Bit().constData(), 0) == 0) {
				append(dbDesc);
			} else {
				dbDesc->destroy();
			}
		}
	}

	beginResetModel();
	qSort(dbDescriptions.begin(), dbDescriptions.end(), &compareDbDescriptionNameLessThan);
	endResetModel();
}

DatabaseDescriptionListModel::~DatabaseDescriptionListModel() {
	Settings::getSettings()->remove(optionGroupName + "/filepath");
	for(int i = 0; i < dbDescriptions.size(); i++)
		Settings::getSettings()->setValue((optionGroupName + "/filepath/%1").arg(i), QString::fromLocal8Bit(dbDescriptions.at(i)->getFilename()));
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
	return 5;
}

QVariant DatabaseDescriptionListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();

	switch(section) {
		case 0: return QVariant("Name");
		case 1: return QVariant("File name");
		case 2: return QVariant("Default DB file base name");
		case 3: return QVariant("Default DB table");
		case 4: return QVariant("Default RDB hash");
	}

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

	switch(index.column()) {
		case 0: return QFileInfo(QString::fromLocal8Bit(dbDescriptions.at(index.row())->getFilename())).fileName();
		case 1: return QString::fromLocal8Bit(dbDescriptions.at(index.row())->getFilename());
		case 2: return QString::fromLocal8Bit(dbDescriptions.at(index.row())->getDefaultFileName());
		case 3: return QString::fromLocal8Bit(dbDescriptions.at(index.row())->getDefaultTableName());
		case 4: {
			QByteArray defaultFileName = QByteArray(dbDescriptions.at(index.row())->getDefaultFileName()) + ".rdb";
			QByteArray hash(defaultFileName.size() + 2, 0);
			convertNameToHash(defaultFileName.constData(), hash.data(), LEGACY_SEED);
			return QString::fromLocal8Bit(hash);
		}
	}

	return QVariant();
}
