#include "RDBSkillTreeSource.h"
#include "RowManipulator.h"
#include <stdio.h>

namespace RappelzRDBBase {

int RDBSkillTreeSource::prepareRead(IRowManipulator* row) {
	int result = RDBSource::prepareRead(row);
	if(result == 0) {
		fread(&subNumber, 1, 4, rdbFile);
		totalRecordNumber = getRowNumber();
		setRowNumber(0);  // can't get total row number :/
		recordNumber = 0;
		return 0;
	}
	return result;
}

int RDBSkillTreeSource::prepareWrite(IRowManipulator* row, unsigned int rowCount) {
	int result = RDBSource::prepareWrite(row, rowCount);

	lastJobId = -1;
	recordNumber = 0;
	lastSubNumberPos = -1;
	subNumber = 0;

	return result;
}

int RDBSkillTreeSource::readRow() {
	int result = RDBSource::readRow();
	if(result)
		return result;

	setRowNumber(getRowNumber() + 1);

	subNumber--;

	if(subNumber <= 0) {
		fread(&subNumber, 1, 4, rdbFile);
		recordNumber++;
	}

	return 0;
}

int RDBSkillTreeSource::writeRow() {
	int jobId;
	IRowManipulator* row = getRowManipulator();

	jobId = *static_cast<int*>(row->getValuePtr(0));
	if(lastJobId != jobId) {
		int currentFilePos = ftell(rdbFile);
		int nulValue = 0;
		if(lastSubNumberPos > 0) {
			fseek(rdbFile, lastSubNumberPos, SEEK_SET);
			fwrite(&subNumber, 1, 4, rdbFile);
			subNumber = 0;
			fseek(rdbFile, currentFilePos, SEEK_SET);
		}

		lastSubNumberPos = currentFilePos;
		fwrite(&nulValue, 1, 4, rdbFile);

		lastJobId = jobId;
		recordNumber++;
	}
	subNumber++;

	return RDBSource::writeRow();
}

void RDBSkillTreeSource::close() {
	if(getOpenMode() == OM_Write) {
		if(lastSubNumberPos > 0) {
			fseek(rdbFile, lastSubNumberPos, SEEK_SET);
			fwrite(&subNumber, 1, 4, rdbFile);
		}
		fseek(rdbFile, 0x80, SEEK_SET);
		fwrite(&recordNumber, 1, 4, rdbFile);
	}
	RDBSource::close();
}

bool RDBSkillTreeSource::hasNext() {
	return !feof(rdbFile) && recordNumber < totalRecordNumber;
}

}  // namespace RappelzRDBBase
