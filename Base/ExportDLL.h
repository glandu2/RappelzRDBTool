#ifndef EXPORTDLL_H_INCLUDED
#define EXPORTDLL_H_INCLUDED

#ifdef __unix__
#	if defined(BUILDING_DATABASE)
#		define EDATABASEDLL __attribute__((visibility("default")))
#		define EBASEDLL
#	elif defined(BUILDING_BASE)
#		define EBASEDLL __attribute__((visibility("default")))
#	else
#		define EBASEDLL
#	endif
#else
#	if defined(BUILDING_DATABASE)
#		define EDATABASEDLL __declspec(dllexport)
#		define EBASEDLL __declspec(dllimport)
#	elif defined(BUILDING_BASE)
#		define EBASEDLL __declspec(dllexport)
#	else
#		define EBASEDLL __declspec(dllimport)
#	endif
#endif

#ifdef __unix__
#	define DLLCALLCONV
#else
#	define DLLCALLCONV __stdcall
#endif


#endif // EXPORTDLL_H_INCLUDED
