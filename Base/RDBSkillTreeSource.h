#ifndef RDBSKILLTREESOURCE_H
#define RDBSKILLTREESOURCE_H

#include "RDBSource.h"

namespace RappelzRDBBase {

class RDBSkillTreeSource : public RDBSource
{
	public:
		virtual int prepareRead(IRowManipulator *row);
		virtual int prepareWrite(IRowManipulator *row, unsigned int rowCount);
		virtual int readRow();
		virtual int writeRow();
		virtual void close();
		virtual bool hasNext();

	protected:
	private:
		int subNumber;
		int lastSubNumberPos;
		int lastJobId;
		int totalRecordNumber;
		int recordNumber;
};

} //namespace

#endif // RDBSKILLTREESOURCE_H
