#include "Database.h"

#include "IDatabaseDescription.h"
#include "Log.h"
#include "SpecialDatabaseRules.h"

#include "CSVSource.h"
#include "RDBSkillTreeSource.h"
#include "RDBSource.h"
#include "SQLFileSource.h"
#include "SQLPostgres.h"
#include "SQLServer.h"
#include "SQLSource.h"

#include "RowManipulator.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

namespace RappelzRDBBase {

FieldOrder Database::getFieldOrderFromColumnName(DataDescriptor* dataDescription, const char* columns) {
	FieldOrder fo;
	size_t i;
	const char* p;

	if(!columns) {
		fo.size = dataDescription->numFields;
		fo.order = (int*) malloc(fo.size * sizeof(int));
		for(i = 0; i < fo.size; i++)
			fo.order[i] = i;
	} else {
		fo.size = dataDescription->numFields;
		fo.order = (int*) malloc(fo.size * sizeof(int));

		std::vector<int> remainingIndex;
		remainingIndex.reserve(fo.size);

		for(i = 0; i < fo.size; i++)
			remainingIndex.push_back(i);

		int inOrder = 0;
		for(p = columns; *p; p += strlen(p) + 1) {
			for(i = 0; i < remainingIndex.size(); i++) {
				if(!strcmp(p, dataDescription->fieldsInfo[remainingIndex[i]].columnName)) {
					fo.order[inOrder] = remainingIndex[i];
					inOrder++;
					remainingIndex.erase(remainingIndex.begin() + i);
					break;
				}
			}
		}

		for(i = 0; i < remainingIndex.size(); i++) {
			if((dataDescription->fieldsInfo[remainingIndex[i]].dataType & TYPE_TYPEMASK) != TYPE_VARCHAR_SIZE) {
				fo.order[inOrder] = remainingIndex[i];
				inOrder++;
			}
		}

		fo.size = inOrder;
	}

	return fo;
}

Database::Database(IDatabaseDescription* databaseDescription) {
	this->databaseDescription = databaseDescription;
	this->dataDescription.numFields = 0;
	this->dataDescription.fieldsInfo = NULL;
	this->rowManipulator = NULL;
	this->dataList = new std::vector<void**>;
	this->userData = 0;
	this->date = 0;

	this->databaseDescription->registerDBStructure(&this->dataDescription.fieldsInfo, &this->dataDescription.numFields);

	this->sqlOrder = getFieldOrderFromColumnName(&this->dataDescription, databaseDescription->getSQLColumnOrder());
	this->csvOrder = getFieldOrderFromColumnName(&this->dataDescription, databaseDescription->getCSVColumnOrder());
	this->rowManipulator = new RowManipulator(&this->dataDescription);
}

Database::~Database() {
	close(NULL);
	if(rowManipulator)
		delete rowManipulator;
	delete dataList;
}

const FieldOrder* Database::getFieldOrder(eDataSourceType type) {
	switch(type) {
		case DST_RDB:
			return NULL;
		case DST_CSV:
			return &csvOrder;
		case DST_SQLPostgres:
			return &sqlOrder;
		case DST_SQLServer:
			return &sqlOrder;
		case DST_SQLFile:
			return &sqlOrder;
	}

	return NULL;
}

void Database::getDataSourceInfo(eDataSourceType type,
                                 IDataSource** ds,
                                 FieldOrder** associatedOrder,
                                 eDataFormat* associatedFormat) {
	switch(type) {
		case DST_RDB:
			if(ds) {
				if(databaseDescription->getSpecialCaseID() == SPECIALCASE_DOUBLEFORRDB)
					*ds = new RDBSkillTreeSource;
				else
					*ds = new RDBSource;
			}
			if(associatedOrder)
				*associatedOrder = NULL;
			if(associatedFormat)
				*associatedFormat = DF_RDB;
			break;

		case DST_CSV:
			if(ds)
				*ds = new CSVSource;
			if(associatedOrder)
				*associatedOrder = &csvOrder;
			if(associatedFormat)
				*associatedFormat = DF_CSV;
			break;

		case DST_SQLFile:
			if(ds)
				*ds = new SQLFileSource(new SQLServer);
			if(associatedOrder)
				*associatedOrder = &sqlOrder;
			if(associatedFormat)
				*associatedFormat = DF_SQL;
			break;

		case DST_SQLPostgres:
			if(ds)
				*ds = new SQLSource(new SQLPostgres);
			if(associatedOrder)
				*associatedOrder = &sqlOrder;
			if(associatedFormat)
				*associatedFormat = DF_SQL;
			break;

		case DST_SQLServer:
			if(ds)
				*ds = new SQLSource(new SQLServer);
			if(associatedOrder)
				*associatedOrder = &sqlOrder;
			if(associatedFormat)
				*associatedFormat = DF_SQL;
			break;
	}
}

/*********************************************************************/

int Database::readData(eDataSourceType type,
                       const char* source,
                       void(DLLCALLCONV* progressCallBack)(void*, int, int),
                       void* arg,
                       const char* location,
                       const char* options) {
	IDataSource* ds;
	int result;
	FieldOrder* order;
	eDataFormat destFormat = DF_None;

	getDataSourceInfo(type, &ds, &order, &destFormat);
	getRowManipulator()->setFieldOrder(order);

	getLogger()->log(ILog::LL_Info, "Reading %s\n", source);

	rowManipulator->setFieldOrder(order);

	result = ds->open(source, IDataSource::OM_Read, location, options);
	if(result)
		return result;

	result = ds->prepare(rowManipulator);
	if(result)
		return result;

	int rowProceed = 0;
	date = ds->getDate();

	try {
		if(ds->getRowNumber() > 500000000) {  // si plus de 500 millions de ligne, alors c'est trop douteux on considere
			                                  // que ce n'est pas un vrai RDB
			getLogger()->log(ILog::LL_Error,
			                 "Database: Too many lines in file %s, is the file format correct ? (got %d lines)\n",
			                 source,
			                 ds->getRowNumber());
			return EINVAL;
		}
		dataList->reserve(ds->getRowNumber());

		while(ds->hasNext()) {
			result = rowManipulator->allocRow();
			if(result)
				return result;

			result = ds->processRow();
			if(result != 0) {
				rowManipulator->freeRow();
				getLogger()->log(ILog::LL_Error, "\nError while reading (%d), aborting\n", result);
				break;
			}
			result = rowManipulator->completeRowInit();
			if(result)
				return result;

			databaseDescription->convertData(destFormat, DCT_Read, rowManipulator, rowProceed);

			dataList->push_back(rowManipulator->getCurrentRow());

			if(progressCallBack)
				progressCallBack(arg, rowProceed, ds->getRowNumber());
			rowProceed++;
		}
		if(result == 0 && progressCallBack)
			progressCallBack(arg, ds->getRowNumber(), ds->getRowNumber());
	} catch(...) {
		getLogger()->log(
		    ILog::LL_Error, "Database: Got exception while reading %s at line %d\n", source, rowProceed + 1);
		return EINVAL;
	}

	ds->close();

	delete ds;

	return result;
}

int Database::writeData(eDataSourceType type,
                        const char* source,
                        void(DLLCALLCONV* progressCallBack)(void*, int, int),
                        void* arg,
                        const char* location,
                        const char* options) {
	IDataSource* ds;
	int result;
	FieldOrder* order;
	eDataFormat destFormat = DF_None;
	int i, recordNumber = dataList->size();

	getDataSourceInfo(type, &ds, &order, &destFormat);
	getRowManipulator()->setFieldOrder(order);

	getLogger()->log(ILog::LL_Info, "Writing %s\n", source);

	rowManipulator->setFieldOrder(order);

	result = ds->open(source, IDataSource::OM_Write, location, options);
	if(result)
		return result;

	result = ds->prepare(rowManipulator, recordNumber);
	if(result)
		return result;

	std::vector<void**>::const_iterator it;

	for(i = 0, it = dataList->begin(); it != dataList->end(); ++it, ++i) {
		rowManipulator->setCurrentRow(*it);

		databaseDescription->convertData(destFormat, DCT_Write, rowManipulator, i);

		result = ds->processRow();
		if(result != 0) {
			getLogger()->log(ILog::LL_Error, "\nError while writing (%d), aborting\n", result);
			break;
		}

		if(progressCallBack)
			progressCallBack(arg, i, recordNumber);
	}
	if(result == 0 && progressCallBack)
		progressCallBack(arg, recordNumber, recordNumber);

	ds->close();

	delete ds;

	for(i = 0, it = dataList->begin(); it != dataList->end(); ++it, ++i) {
		rowManipulator->setCurrentRow(*it);
		databaseDescription->convertData(destFormat, DCT_Read, rowManipulator, i);
	}

	return result;
}

void Database::close(void(DLLCALLCONV* progressCallBack)(void*, int, int), void* arg) {
	int i, recordNumber = dataList->size();
	std::vector<void**>::iterator it;

	for(i = 0, it = dataList->begin(); it != dataList->end(); ++it, ++i) {
		rowManipulator->setCurrentRow(*it);
		rowManipulator->freeRow();

		if(progressCallBack)
			progressCallBack(arg, i, recordNumber);
	}

	dataList->clear();
	std::vector<void**>().swap(*dataList);  // clear memeory
	if(progressCallBack)
		progressCallBack(arg, recordNumber, recordNumber);
}

IRowManipulator* Database::getRowManipulator() {
	return rowManipulator;
}

const std::vector<void**>* Database::getRows() {
	return dataList;
}

void Database::setUserData(void* data) {
	userData = data;
}

void* Database::getUserData() {
	return userData;
}

}  // namespace RappelzRDBBase
