#include "ICharsetConverter.h"
#include "CharsetConverter.h"

#pragma comment(linker, "/EXPORT:createCharsetConverter=_createCharsetConverter@4")
ICharsetConverter * DLLCALLCONV createCharsetConverter(const char* charset) {
	try {
		return new CharsetConverter(charset);
	} catch(...) {
		return 0;
	}
}

#pragma comment(linker, "/EXPORT:availableCharsets=_availableCharsets@0")
const CharsetInfo * DLLCALLCONV availableCharsets() {
	static const CharsetInfo charsets[] = {
		{"CP1250", "Central Europe"},
		{"CP1251", "Cyrillic"},
		{"CP1252", "Western Europe"},
		{"CP1254", "Turkish"},
		{"CP1256", "Arabic"},
		{"CP874", "Thai"},
		{"CP932", "Japanese"},
		{"CP936", "Chinese simplified"},
		{"CP949", "Korean"},
		{"CP950", "Chinese traditional"},
		{0, 0}
	};

	return charsets;
}
