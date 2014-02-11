#ifndef CHARSETCONVERTER_H
#define CHARSETCONVERTER_H

#include "iconv.h"

#include "ICharsetConverter.h"

class CharsetConverter : public CImplement<ICharsetConverter>
{
public:
	CharsetConverter(const char* charset);
	~CharsetConverter();

	virtual int toUtf16(const char** inbuf, int* inSize, char** outbuf, int* outSize);
	virtual int fromUtf16(const char** inbuf, int* inSize, char** outbuf, int* outSize);

private:
	iconv_t ic_toUtf16;
	iconv_t ic_fromUtf16;
};

#endif // CHARSETCONVERTER_H
