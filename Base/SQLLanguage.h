#ifndef SQLLANGUAGE_H_INCLUDED
#define SQLLANGUAGE_H_INCLUDED

namespace RappelzRDBBase {

class SQLLanguage
{
	public:
		virtual void getTypeName(char *buffer, int type, int count = 1, int index = 0) = 0;
};

} //namespace

#endif // SQLLANGUAGE_H_INCLUDED
