#if defined(BUILDING_LIBICONV) && defined(_WIN32)
	/* Building shared library. */
#  define LIBICONV_DLL_EXPORTED __declspec(dllexport)
#elif defined(USING_ICONV_SHARED) && defined(_WIN32)
	/* Using shared library. */
#  define LIBICONV_DLL_EXPORTED __declspec(dllimport)
#elif (defined(BUILDING_LIBICONV) || defined(USING_ICONV_SHARED)) && __GNUC__ >= 4
	/* Building shared library. */
#  define LIBICONV_DLL_EXPORTED __attribute__((visibility("default")))
#else
	/* Building static library. */
#  define LIBICONV_DLL_EXPORTED /* nothing */
#endif
