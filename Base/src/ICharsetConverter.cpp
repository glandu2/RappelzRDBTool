#include "ICharsetConverter.h"
#include "CharsetConverter.h"

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:createCharsetConverter=_createCharsetConverter@4")
#endif
ICharsetConverter * DLLCALLCONV createCharsetConverter(const char* charset) {
	try {
		return new CharsetConverter(charset);
	} catch(...) {
		return 0;
	}
}

#ifndef _WIN64
#pragma comment(linker, "/EXPORT:availableCharsets=_availableCharsets@0")
#endif
const CharsetInfo * DLLCALLCONV availableCharsets() {
	static const CharsetInfo charsets[] = {
		{"CP1252", "Western Europe"},
		{"CP1256", "Arabic"},
		{"CP1251", "Cyrillic"},
		{"CP949", "Korean"},
		{"CP1250", "Central Europe"},
		{"CP1254", "Turkish"},
		{"CP932", "Japanese"},
		{"CP936", "Chinese simplified"},
		{"CP950", "Chinese traditional"},
		{"CP874", "Thai"},
		{0, 0}
	};

	return charsets;
}
