#ifndef _BOOZE_EXPORT_H_
#define _BOOZE_EXPORT_H_

#if defined _WIN32 || defined _WIN64
	#ifdef BOOZE_STATIC_LIB
		#define BOOZE_EXPORT extern "C"
	#else
#define BOOZE_EXPORT extern "C" __declspec(dllexport)
	#endif
#else
#ifdef BOOZE_STATIC_LIB
#define BOOZE_EXPORT extern "C"
	#else
#define BOOZE_EXPORT extern "C" __attribute__ ((visibility ("default")))
	#endif
#endif

#endif /* _BOOZE_EXPORT_H_ */

