#include "Database.h"

#include "IDatabaseDescription.h"
#include "SpecialDatabaseRules.h"

#include "RDBSource.h"
#include "RDBSkillTreeSource.h"
#include "CSVSource.h"
#include "SQLSource.h"
#include "SQLPostgres.h"
#include "SQLServer.h"

#include "RowManipulator.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <string.h>

namespace RappelzRDBBase {

FieldOrder Database::getFieldOrderFromColumnName(DataDescriptor *dataDescription, const char *columns) {
	FieldOrder fo;
	int i, count;
	const char *p;

	if(!columns) {
		fo.size = dataDescription->numFields;
		fo.order = (int*) malloc(fo.size * sizeof(int));
		for(i = 0;i < fo.size ;i++) fo.order[i] = i;
	} else {
		fo.order = (int*) malloc(dataDescription->numFields * sizeof(int));
		for(count=0, p=columns; *p; p += strlen(p) + 1) {
			for(i = 0; i < dataDescription->numFields; i++) {
				if(!strcmp(p, dataDescription->fieldsInfo[i].columnName)) {
					fo.order[count] = i;
					count++;
					break;
				}
			}
		}

		fo.size = count;
	}

	return fo;
}

Database::Database(IDatabaseDescription *databaseDescription) {
	this->databaseDescription = databaseDescription;
	this->dataDescription.numFields = 0;
	this->dataDescription.fieldsInfo = NULL;
	this->rowManipulator = NULL;
	this->dataList = new std::vector<void**>;
	this->userData = 0;

	this->databaseDescription->registerDBStructure(&this->dataDescription.fieldsInfo, &this->dataDescription.numFields);

	this->sqlOrder = getFieldOrderFromColumnName(&this->dataDescription, databaseDescription->getSQLColumnOrder());
	this->csvOrder = getFieldOrderFromColumnName(&this->dataDescription, databaseDescription->getCSVColumnOrder());
	this->rowManipulator = new RowManipulator(&this->dataDescription);
}

Database::~Database() {
	close(NULL);
	if(rowManipulator) delete rowManipulator;
	delete dataList;
}

const FieldOrder *Database::getFieldOrder(eDataSourceType type) {
	switch(type) {
		case DST_RDB: return NULL;
		case DST_CSV: return &csvOrder;
		case DST_SQLPostgres: return &sqlOrder;
		case DST_SQLServer: return &sqlOrder;
	}

	return NULL;
}

void Database::getDataSourceInfo(eDataSourceType type, IDataSource **ds, FieldOrder **associatedOrder, eDataFormat *associatedFormat) {
	switch(type) {
		case DST_RDB:
			if(ds) {
				if(databaseDescription->getSpecialCaseID() == SPECIALCASE_DOUBLEFORRDB)
					*ds = new RDBSkillTreeSource;
				else *ds = new RDBSource;
			}
			if(associatedOrder) *associatedOrder = NULL;
			if(associatedFormat) *associatedFormat = DF_RDB;
			break;

		case DST_CSV:
			if(ds) *ds = new CSVSource;
			if(associatedOrder) *associatedOrder = &csvOrder;
			if(associatedFormat) *associatedFormat = DF_CSV;
			break;

		case DST_SQLPostgres:
			if(ds) *ds = new SQLSource(new SQLPostgres);
			if(associatedOrder) *associatedOrder = &sqlOrder;
			if(associatedFormat) *associatedFormat = DF_SQL;
			break;

		case DST_SQLServer:
			if(ds) *ds = new SQLSource(new SQLServer);
			if(associatedOrder) *associatedOrder = &sqlOrder;
			if(associatedFormat) *associatedFormat = DF_SQL;
			break;
	}
}

/*********************************************************************/

int Database::readData(eDataSourceType type, const char* source, void (DLLCALLCONV *progressCallBack)(void*, int, int), void *arg, const char* location, const char* user, const char* password) {
	IDataSource *ds;
	int result;
	FieldOrder *order;
	eDataFormat destFormat = DF_None;

	getDataSourceInfo(type, &ds, &order, &destFormat);
	getRowManipulator()->setFieldOrder(order);

	fprintf(stderr, "Reading %s\n", source);

	rowManipulator->setFieldOrder(order);

	result = ds->open(source, IDataSource::OM_Read, location, user, password);
	if(result) return result;

	result = ds->prepare(rowManipulator);
	if(result) return result;

	int rowProceed = 0;

	dataList->reserve(ds->getRowNumber());

	while(ds->hasNext()) {
		result = rowManipulator->allocRow();
		if(result) return result;

		result = ds->processRow();
		if(result != 0) {
			rowManipulator->freeRow();
			fprintf(stderr, "\nError while reading (%d), aborting\n", result);
			break;
		}
		result = rowManipulator->completeRowInit();
		if(result) return result;

		databaseDescription->convertData(destFormat, DCT_Read, rowManipulator, rowProceed);

		dataList->push_back(rowManipulator->getCurrentRow());

		if(progressCallBack) progressCallBack(arg, rowProceed, ds->getRowNumber());
		rowProceed++;
	}
	if(result == 0 && progressCallBack) progressCallBack(arg, ds->getRowNumber(), ds->getRowNumber());

	ds->close();

	delete ds;

	return 0;
}


int Database::writeData(eDataSourceType type, const char* source, void (DLLCALLCONV *progressCallBack)(void*, int, int), void *arg, const char* location, const char* user, const char* password) {
	IDataSource *ds;
	int result;
	FieldOrder *order;
	eDataFormat destFormat = DF_None;
	int i, recordNumber = dataList->size();

	getDataSourceInfo(type, &ds, &order, &destFormat);
	getRowManipulator()->setFieldOrder(order);

	fprintf(stderr, "Writing %s\n", source);

	rowManipulator->setFieldOrder(order);

	result = ds->open(source, IDataSource::OM_Write, location, user, password);
	if(result) return result;

	result = ds->prepare(rowManipulator, recordNumber);
	if(result) return result;

	std::vector<void**>::const_iterator it;

	for(i=0, it = dataList->begin(); it != dataList->end(); ++it, ++i) {
		rowManipulator->setCurrentRow(*it);

		databaseDescription->convertData(destFormat, DCT_Write, rowManipulator, i);

		result = ds->processRow();
		if(result != 0) {
			fprintf(stderr, "\nError while writing (%d), aborting\n", result);
			break;
		}

		if(progressCallBack) progressCallBack(arg, i, recordNumber);
	}
	if(result == 0 && progressCallBack) progressCallBack(arg, recordNumber, recordNumber);

	ds->close();

	delete ds;

	return 0;
}

void Database::close(void (DLLCALLCONV *progressCallBack)(void*, int, int), void *arg) {
	int i, recordNumber = dataList->size();
	std::vector<void**>::iterator it;

	for(i=0, it = dataList->begin(); it != dataList->end(); ++it, ++i) {
		rowManipulator->setCurrentRow(*it);
		rowManipulator->freeRow();

		if(progressCallBack) progressCallBack(arg, i, recordNumber);
	}

	dataList->clear();
	std::vector<void**>().swap(*dataList);	//clear memeory
	if(progressCallBack) progressCallBack(arg, recordNumber, recordNumber);
}

IRowManipulator *Database::getRowManipulator() {
	return rowManipulator;
}

const std::vector<void**> * Database::getRows() {
	return dataList;
}

void Database::setUserData(void* data) {
	userData = data;
}

void* Database::getUserData() {
	return userData;
}

} //namespace
