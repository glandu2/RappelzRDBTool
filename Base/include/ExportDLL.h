#ifndef EXPORTDLL_H_INCLUDED
#define EXPORTDLL_H_INCLUDED

#ifdef __unix__
#	if defined(BUILDING_BASE)
#		define EBASEDLL __attribute__((visibility("default")))
#	else
#		define EBASEDLL
#	endif
#	define EDATABASEDLL __attribute__((visibility("default")))
#else
#	if defined(BUILDING_BASE)
#		define EBASEDLL __declspec(dllexport)
#	else
#		define EBASEDLL __declspec(dllimport)
#	endif
#	define EDATABASEDLL __declspec(dllexport)
#endif

#ifdef __unix__
#	define DLLCALLCONV
#else
#	define DLLCALLCONV __stdcall
#endif

#ifdef __GNUC__
#define PRINTFCHECK(formatidx, argsidx) __attribute__((format (printf, formatidx, argsidx)))
#else
#define PRINTFCHECK(formatidx, argsidx)
#endif


#endif // EXPORTDLL_H_INCLUDED
