#ifndef ICHARSETCONVERTER_H
#define ICHARSETCONVERTER_H

#include "IObject.h"
#include <stdlib.h>
#include <errno.h>

class ICharsetConverter;

#ifdef __cplusplus
extern "C" {
#endif

struct CharsetInfo {
	const char* name;
	const char* description;
};

EBASEDLL ICharsetConverter * DLLCALLCONV createCharsetConverter(const char* charset);
EBASEDLL const struct CharsetInfo * DLLCALLCONV availableCharsets();

#ifdef __cplusplus
}
#endif

class ICharsetConverter : public IObject
{
public:
	struct ConvertedString {
		char* data;
		int size;
	};

	//Convert input to out. Return error
	//Invalid or non convertible characters are replaced by ?
	//only utf16le is used
	virtual int toUtf16(const char** inbuf, int* inSize, char** outbuf, int* outSize) = 0;
	virtual int fromUtf16(const char** inbuf, int* inSize, char** outbuf, int* outSize) = 0;

	//return number of significant bytes in converted buffer
	inline int convertToUtf16(ConvertedString in, ConvertedString *converted) {
		ConvertedString out;

		if(in.size == 0) {
			out.size = 2;
			out.data = (char*)calloc(1, out.size);
			*converted = out;
			return out.size;
		}

		out.size = in.size * 2;
		out.data = (char*) malloc(out.size);

		const char* inPtr = in.data;
		char* outPtr = out.data;
		int inRemainingBytes = in.size, outRemainingBytes = out.size;

		while(inRemainingBytes > 0) {
			int result = toUtf16(&inPtr, &inRemainingBytes, &outPtr, &outRemainingBytes);
			if(result < 0) {
				if(result == -E2BIG) {
					int outDone = out.size - outRemainingBytes;
					out.size *= 2;
					out.data = (char*)realloc(out.data, out.size);
					outPtr = out.data + outDone;
					outRemainingBytes = out.size - outDone;
				} else {
					break;
				}
			}
		}

		out.size -= outRemainingBytes;
		*converted = out;
		return out.size;
	}

	//return number of significant bytes in converted buffer
	inline int convertFromUtf16(ConvertedString in, ConvertedString *converted) {
		ConvertedString out;

		if(in.size == 0) {
			out.size = 1;
			out.data = (char*)calloc(1, out.size);
			*converted = out;
			return out.size;
		}

		out.size = in.size;
		out.data = (char*) malloc(out.size);

		const char* inPtr = in.data;
		char* outPtr = out.data;
		int inRemainingBytes = in.size, outRemainingBytes = out.size;

		while(inRemainingBytes > 1) {
			int result = fromUtf16(&inPtr, &inRemainingBytes, &outPtr, &outRemainingBytes);
			if(result < 0) {
				if(result == -E2BIG) {
					int outDone = out.size - outRemainingBytes;
					out.size *= 2;
					out.data = (char*)realloc(out.data, out.size);
					outPtr = out.data + outDone;
					outRemainingBytes = out.size - outDone;
				} else {
					break;
				}
			}
		}

		out.size -= outRemainingBytes;
		*converted = out;
		return out.size;
	}
};

#endif // ICHARSETCONVERTER_H
