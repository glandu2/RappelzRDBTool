#include "CharsetConverter.h"
#include <errno.h>
#include <stdio.h>
#include "Log.h"

CharsetConverter::CharsetConverter(const char *charset) {
	ic_toUtf16 = iconv_open("UTF-16LE", charset);
	if(!ic_toUtf16 || ic_toUtf16 == (iconv_t)-1) {
		getLogger()->log(ILog::LL_Fatal, "Cant open charset \"%s\"\n", charset);
		throw errno;
	}

	ic_fromUtf16 = iconv_open(charset, "UTF-16LE");
	if(!ic_fromUtf16 || ic_fromUtf16 == (iconv_t)-1) {
		iconv_close(ic_toUtf16);
		getLogger()->log(ILog::LL_Fatal, "Cant open charset \"%s\"\n", charset);
		throw errno;
	}
}

CharsetConverter::~CharsetConverter() {
	iconv_close(ic_toUtf16);
	iconv_close(ic_fromUtf16);
}

int CharsetConverter::toUtf16(const char** inBuf, int* inSize, char** outBuf, int* outSize) {
	size_t sInSize = *inSize;
	size_t sOutSize = *outSize;
	const char* inPtr = *inBuf;
	char* outPtr = *outBuf;

	iconv(ic_toUtf16,NULL,NULL,NULL,NULL);
	while(iconv(ic_toUtf16, &inPtr, &sInSize, &outPtr, &sOutSize) == -1 && sInSize > 0 && sOutSize > 1) {
		if(errno == EINVAL || errno == E2BIG)
			break;

		inPtr ++;
		sInSize --;

		*outPtr = '?';
		outPtr++;
		*outPtr = '\0';
		outPtr++;

		sOutSize -= 2;
	}

	*inBuf = inPtr;
	*inSize = (int)sInSize;
	*outBuf = outPtr;
	*outSize = (int)sOutSize;

	if(sInSize == 0)
		return 0;
	else {
		if(errno == EINVAL || errno == E2BIG)
			return -errno;
		else
			return -EINVAL;
	}
}

int CharsetConverter::fromUtf16(const char** inBuf, int* inSize, char** outBuf, int* outSize) {
	size_t sInSize = *inSize;
	size_t sOutSize = *outSize;
	const char* inPtr = *inBuf;
	char* outPtr = *outBuf;
	size_t result;

	iconv(ic_fromUtf16,NULL,NULL,NULL,NULL);
	while((result = iconv(ic_fromUtf16, &inPtr, &sInSize, &outPtr, &sOutSize)) == -1 && sInSize > 1 && sOutSize > 0) {
		if(errno == EINVAL || errno == E2BIG)
			break;

		inPtr += 2;
		sInSize -= 2;

		*outPtr = '?';
		outPtr++;

		sOutSize--;
	}

	*inBuf = inPtr;
	*inSize = (int)sInSize;
	*outBuf = outPtr;
	*outSize = (int)sOutSize;

	if(sInSize == 0)
		return 0;
	else {
		if(errno == EINVAL || errno == E2BIG)
			return -errno;
		else
			return -EINVAL;
	}
}
