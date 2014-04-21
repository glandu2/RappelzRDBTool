#include "FileToRenameListModel.h"
#include "NameToHash.h"
#include "HashToName.h"
#include "Settings.h"
#include <QFileInfo>
#include <QRegExp>

FileToRenameListModel::FileToRenameListModel(QObject *parent) :
	QAbstractTableModel(parent)
{
	int i = 0;
	QString value;

#ifdef __linux__
	optionGroupName = "FilesToRenameLinux";
#else
	optionGroupName = "FilesToRenameWindows";
#endif

	while((value = Settings::getSettings()->value((optionGroupName + "/filepath/%1").arg(i)).toString()).isNull() == false) {
		QFileInfo targetFileInfo(value);

		FileInfo fileInfo;
		fileInfo.filename = targetFileInfo.absoluteFilePath();
		fileInfo.isHashed = isHashedName(targetFileInfo.fileName());
		fileList.append(fileInfo);

		i++;
	}
}

FileToRenameListModel::~FileToRenameListModel() {
	Settings::getSettings()->remove(optionGroupName + "/filepath");
	for(int i = 0; i < fileList.size(); i++)
		Settings::getSettings()->setValue((optionGroupName + "/filepath/%1").arg(i), fileList.at(i).filename);
}

void FileToRenameListModel::append(QString filename) {
	QFileInfo targetFileInfo(filename);
	QString absoluteFilename = targetFileInfo.absoluteFilePath();
	FileInfo fileInfo;

	foreach(fileInfo, fileList) {
		if(fileInfo.filename == absoluteFilename)
			return;
	}

	if(targetFileInfo.isFile() && targetFileInfo.exists()) {
		fileInfo.filename = absoluteFilename;
		fileInfo.isHashed = isHashedName(targetFileInfo.fileName());

		beginInsertRows(QModelIndex(), fileList.size(), fileList.size());
		fileList.append(fileInfo);
		endInsertRows();
	}
}

void FileToRenameListModel::remove(int index) {
	if(index != -1) {
		beginRemoveRows(QModelIndex(), index, index);
		fileList.removeAt(index);
		endRemoveRows();
	}
}

QString FileToRenameListModel::getSourceFilename(int index) {
	return fileList.at(index).filename;
}

QString FileToRenameListModel::getTargetFilename(int index) {
	return convertFilename(fileList.at(index));
}


int FileToRenameListModel::columnCount(const QModelIndex& parent) const {
	if(parent.isValid())
		return 0;
	return 3;
}

QVariant FileToRenameListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(orientation == Qt::Vertical || role != Qt::DisplayRole)
		return QVariant();

	switch(section) {
		case 0: return tr("Type", "column name in file to rename dialogbox");
		case 1: return tr("File path", "column name in file to rename dialogbox");
		case 2: return tr("Converted", "column name in file to rename dialogbox");
	}

	return QVariant();
}

int FileToRenameListModel::rowCount(const QModelIndex& parent) const {
	if(parent.isValid())
		return 0;
	return fileList.size();
}

QVariant FileToRenameListModel::data(const QModelIndex& index, int role) const {
	if(!index.isValid() || index.parent().isValid() || role != Qt::DisplayRole)
		return QVariant();

	switch(index.column()) {
		case 0: return fileList.at(index.row()).isHashed ? tr("Hashed", "Filename type") : tr("Not hashed", "Filename type");
		case 1: return fileList.at(index.row()).filename;
		case 2: return convertFilename(fileList.at(index.row()));
	}

	return QVariant();
}

QString FileToRenameListModel::convertFilename(FileInfo fileInfo) {
	QFileInfo targetFileInfo(fileInfo.filename);

	if(fileInfo.isHashed) {
		QByteArray hash = targetFileInfo.fileName().toLatin1();
		QByteArray name;

		if(hash.size() >= 2) {
			name.resize(hash.size()-1);
			convertHashToName(hash.constData(), name.data());
			name.resize(name.size()-1);

			return targetFileInfo.absolutePath() + "/" + name;
		}
	} else {
		QByteArray name = targetFileInfo.fileName().toLatin1();
		QByteArray hash;

		hash.resize(name.size()+3);
		convertNameToHash(name.constData(), hash.data(), LEGACY_SEED);
		hash.resize(hash.size()-1);

		return targetFileInfo.absolutePath() + "/" + hash;
	}

	return QString();
}

bool FileToRenameListModel::isHashedName(QString filename) {
	static QRegExp invalidCharsForName("[^ _\\-\\!\\.\\(\\)@&0-9a-zA-Z]");
	static const char* knownExtension[] = {
		"bmp", "cfg", "cob", "dds",
		"dmp", "gc2", "gci", "ini",
		"jpg", "jtv", "lst", "lua",
		"m4v", "max", "naf", "nfa",
		"nfc", "nfe", "nfk", "nfl",
		"nfm", "nfp", "nfs", "nfw",
		"nui", "nx3", "obj", "otf",
		"png", "pvs", "qpf", "rdb",
		"rdu", "sdb", "spr", "spt",
		"tga", "tif", "tml", "ttf",
		"txt", "wav", "xml", "fx",
		"db"
	};

	if(filename.size() < 2)
		return false;

	if(!filename.contains(invalidCharsForName)) { //contains no invalid chars
		for(unsigned int i = 0; i < sizeof(knownExtension)/sizeof(const char*); i++) {
			if(filename.endsWith(QString(".") + knownExtension[i])) {
				return false; // has a known extension => it's a name
			}
		}
	}

	return true;
}
