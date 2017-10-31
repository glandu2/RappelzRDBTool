#include "DatabaseTableModel.h"
#include "ICharsetConverter.h"
#include "IDatabase.h"
#include "ILog.h"
#include "IRowManipulator.h"
#include "Settings.h"
#include <errno.h>

#include <QByteArray>
#include <math.h>
#include <stdio.h>
#include <vector>

DatabaseTableModel::DatabaseTableModel(QByteArray charset, QObject* parent) : QAbstractTableModel(parent) {
	db = NULL;
	row = NULL;
	currentLocale = createCharsetConverter(charset.constData());
	if(currentLocale == 0)
		currentLocale = createCharsetConverter("CP1252");
}

void DatabaseTableModel::changeLocale(QByteArray newLocale) {
	ICharsetConverter* newTextLocale = createCharsetConverter(newLocale.constData());
	if(newTextLocale) {
		beginResetModel();
		currentLocale = newTextLocale;
		endResetModel();
	}
}

void DatabaseTableModel::bindToDatabase(IDatabase* db) {
	int i, colCount;

	beginResetModel();

	row = createRowManipulator(db->getRowManipulator()->getColumnType(), 0);

	for(colCount = i = 0; i < row->getColumnCount(); i++) {
		if(row->getType(i) != TYPE_VARCHAR_SIZE && (row->getIgnoreType(i) & TYPE_GUIIGNORE) == 0) {
			columnBinding.resize(colCount + 1);
			columnBinding[colCount] = i;
			colCount++;
		}
	}

	this->db = db;

	endResetModel();
}

void DatabaseTableModel::unbindDatabase() {
	beginResetModel();
	db = NULL;
	if(row)
		row->destroy();
	row = NULL;
	endResetModel();
}

Qt::ItemFlags DatabaseTableModel::flags(const QModelIndex&) const {
	// if(db && index.isValid() && index.column() < columnCount() && index.row() < rowCount())             //Fixme:
	// needed ?
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	// return Qt::NoItemFlags;
}

int DatabaseTableModel::columnCount(const QModelIndex& parent) const {
	if(parent.isValid() || !db)
		return 0;
	else
		return columnBinding.size();
}

int DatabaseTableModel::rowCount(const QModelIndex& parent) const {
	if(parent.isValid() || !db)
		return 0;
	else
		return db->getRowCount();
}

QString DatabaseTableModel::toUnicode(const char* data, int size) const {
	ICharsetConverter::ConvertedString in, out;
	in.data = (char*) data;
	in.size = size;
	currentLocale->convertToUtf16(in, &out);
	QString ret((const QChar*) out.data, out.size / 2);

	free(out.data);

	return ret;
}

QByteArray DatabaseTableModel::fromUnicode(QString data) const {
	ICharsetConverter::ConvertedString in, out;
	in.data = (char*) data.data();
	in.size = data.size() * 2;
	currentLocale->convertFromUtf16(in, &out);
	QByteArray ret(out.data, out.size);

	free(out.data);

	return ret;
}

QVariant DatabaseTableModel::data(const QModelIndex& index, int role) const {
	if(!db || !index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
		return QVariant(QVariant::Invalid);

	row->setCurrentRow(db->getRowAt(index.row()));

	int realColumnIndex = columnBinding.at(index.column());
	int count = row->getDataCount(realColumnIndex);
	void* buffer = row->getValuePtr(realColumnIndex);

	switch(row->getType(realColumnIndex)) {
		case TYPE_BIT: {
			QByteArray item;
			int bitRead = 0;
			for(; count > 0; count--) {
				item.append(QByteArray::number((int) ((*(char*) buffer >> bitRead) & 1)));
				bitRead++;
				if(!(bitRead % 8)) {
					buffer = (char*) buffer + 1;
					bitRead = 0;
				}
			}
			return QVariant::fromValue(item);
			break;
		}

		case TYPE_CHAR: {  // avoid big strings in table
			int i;
			for(i = 0; ((char*) buffer)[i] && i < count; i++)
				;
			return QVariant::fromValue(toUnicode((char*) buffer, i));
			break;
		}

		case TYPE_NVARCHAR_STR:
		case TYPE_VARCHAR_STR:
			if(((char*) buffer)[count - 1] == '\0')  // On ne met pas de 0 final dans le QString
				return QVariant::fromValue(toUnicode((char*) buffer, count - 1));
			else
				return QVariant::fromValue(toUnicode((char*) buffer, count));
			break;

		case TYPE_INT8:
			return QVariant::fromValue((int) *(char*) buffer);
			break;

		case TYPE_INT16:
			return QVariant::fromValue((int) *(short*) buffer);
			break;

		case TYPE_FLOAT32:
			return QVariant::fromValue(*(float*) buffer);
			break;

		case TYPE_DECIMAL:
			return QVariant::fromValue((double) (*(int*) buffer) / pow((float) 10, row->getDataIndex(realColumnIndex)));
			break;

		case TYPE_VARCHAR_SIZE:
			getLogger()->log(ILog::LL_Trace, "Warning ! Attempting to print varchar size\n");
			break;

		case TYPE_INT32:
			return QVariant::fromValue(*(int*) buffer);
			break;

		case TYPE_FLOAT64:
			return QVariant::fromValue(*(double*) buffer);
			break;

		case TYPE_INT64:
			return QVariant::fromValue(*(long long int*) buffer);
			break;
	}

	return QVariant(QVariant::Invalid);
}

QVariant DatabaseTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if(!db || role != Qt::DisplayRole)
		return QVariant(QVariant::Invalid);

	if(orientation == Qt::Horizontal) {
		return QVariant::fromValue(QByteArray::fromRawData(row->getColumnName(columnBinding.at(section)),
		                                                   qstrlen(row->getColumnName(columnBinding.at(section)))));
	} else
		return QVariant::fromValue(section);
}

bool DatabaseTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
	if(db && index.isValid() && index.column() < columnCount() && index.row() < rowCount() && role == Qt::EditRole) {
		if(value.toString() == data(index, role))
			return true;

		row->setCurrentRow(db->getRowAt(index.row()));
		int realColumnIndex = columnBinding.at(index.column());

		QByteArray converted;

		if(row->getType(realColumnIndex) == TYPE_VARCHAR_STR || row->getType(realColumnIndex) == TYPE_NVARCHAR_STR ||
		   row->getType(realColumnIndex) == TYPE_CHAR)
			converted = fromUnicode(value.toString());

		if(row->getType(realColumnIndex) == TYPE_VARCHAR_STR || row->getType(realColumnIndex) == TYPE_NVARCHAR_STR) {
			row->freeValue(realColumnIndex);
			row->initData(realColumnIndex, converted.size() + 1);
		}
		void* buffer = row->getValuePtr(realColumnIndex);
		bool result = false;

		switch(row->getType(realColumnIndex)) {
			case TYPE_INT8:
			case TYPE_BIT: {
				int val = value.toInt(&result);
				if(val & 0xFFFFFF00)
					result = false;
				if(result)
					*(char*) buffer = val;
				break;
			}

			case TYPE_INT16: {
				int val = value.toInt(&result);
				if(val & 0xFFFF0000)
					result = false;
				if(result)
					*(short*) buffer = val;
				break;
			}

			case TYPE_INT32: {
				int val = value.toInt(&result);
				if(result)
					*(int*) buffer = val;
				break;
			}

			case TYPE_DECIMAL: {
				float decimalTemp = value.toFloat(&result);
				if(result)
					*(int*) buffer = (int) (decimalTemp * pow((float) 10, row->getDataIndex(realColumnIndex)) + 0.5);
				break;
			}

			case TYPE_INT64: {
				long long int val = value.toLongLong(&result);
				if(result)
					*(long long int*) buffer = val;
				break;
			}

			case TYPE_CHAR:
				result = true;
				qstrncpy((char*) buffer, converted.constData(), row->getDataCount(realColumnIndex) + 1);
				break;

			case TYPE_FLOAT32: {
				float val = value.toFloat(&result);
				if(result)
					*(float*) buffer = val;
				break;
			}

			case TYPE_FLOAT64: {
				double val = value.toDouble(&result);
				if(result)
					*(double*) buffer = val;
				break;
			}

			case TYPE_NVARCHAR_STR:
			case TYPE_VARCHAR_STR:
				result = true;
				qstrncpy((char*) buffer, converted.constData(), row->getDataCount(realColumnIndex) + 1);
				break;
		}

		if(result)
			emit dataChanged(createIndex(index.row(), index.column()), createIndex(index.row(), index.column()));

		return result;
	}

	return false;
}
