#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#define TYPE_BIT		0x00000000
#define TYPE_INT8		0x00000001
#define	TYPE_INT16		0x00000002
#define TYPE_INT32		0x00000003
#define TYPE_INT64		0x00000004
#define TYPE_FLOAT32	0x00000009
#define TYPE_FLOAT64	0x0000000A
#define TYPE_CHAR		0x0000000B				//dataSize = string length
#define TYPE_VARCHAR_SIZE	0x0000000C
#define TYPE_VARCHAR_STR	0x0000000D
#define TYPE_DECIMAL	0x0000000E			//dataSize = number of fractionnal digits,  here it would be 2: 567323.54

#define TYPE_RDBIGNORE	0x00000100
#define TYPE_SQLIGNORE	0x00000200
#define TYPE_CSVIGNORE	0x00000400
#define TYPE_GUIIGNORE  0x00000800

#define TYPE_FLAG_KEY	0x00010000		//primary key flag

#define TYPE_TYPEMASK	0x000000FF
#define TYPE_IGNOREMASK 0x0000FF00
#define TYPE_FLAGMASK	0xFFFF0000

#define GET_TYPE(t) ((t) & TYPE_TYPEMASK)
#define GET_IGNORETYPE(t) ((t) & TYPE_IGNOREMASK)
#define GET_FLAGS(t) ((t) & TYPE_FLAGMASK)

#define GET_FLAGBIT(t, f) (((t) & (f)) != 0)

#define GET_VAR_INDEX(c) ((c) >> 16)
#define GET_VAR_LNGTH(c) ((c) & 0xFFFF)
#define MAKEINDEXEDVAR(ind, sz) (((sz) & 0xFFFF) | ((ind) << 16))

struct FieldOrder {
	int *order;
	int size;
};
struct FieldDescriptor {
	int dataSize;		//number of data (or stringLength var index)
	int dataType;	//type of data
	const char* columnName;		//index of the coresponding column in SQL table
};
struct DataDescriptor {	//data must be in RDB order
	FieldDescriptor *fieldsInfo;
	int numFields;
};

enum eDataSourceType { DST_RDB=1, DST_CSV=2, DST_SQLServer=3, DST_SQLPostgres=4, DST_SQLFile};
enum eDataFormat { DF_None=0, DF_RDB=1, DF_CSV=2, DF_SQL=3 };
enum eDataConvertionType { DCT_Read=0, DCT_Write=1 };

#endif // DATATYPE_H_INCLUDED
