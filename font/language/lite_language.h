#ifndef __LITE_LANGUAGE_H__
#define __LITE_LANGUAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lite_string_table.h"

#define LITE_S_CHINESE
//#define LITE_T_CHINESE
#define LITE_RUSSIAN
//#define LITE_JAPANESE
//#define LITE_PORTUGUESE
//#define LITE_FRENCH
#define LITE_KOREAN
//#define LITE_ROMANIAN
//#define LITE_HEBREW
//#define LITE_ARABIC
//#define LITE_PERSIAN
//#define LITE_THAI


typedef enum
{
	LITE_LANG_ENG,

	#ifdef	LITE_S_CHINESE
	LITE_LANG_SCH,
	#endif

	#ifdef	LITE_T_CHINESE
	LITE_LANG_TCH,
	#endif

	#ifdef	LITE_RUSSIAN
	LITE_LANG_RUS,
	#endif

	#ifdef	LITE_JAPANESE
	LITE_LANG_JPN,
	#endif

	#ifdef	LITE_PORTUGUESE
	LITE_LANG_PTG,
	#endif

	#ifdef	LITE_FRENCH
	LITE_LANG_FRN,
	#endif

	#ifdef	LITE_KOREAN
	LITE_LANG_KOR,
	#endif

	#ifdef	LITE_ROMANIAN
	LITE_LANG_RMN,
	#endif

	#ifdef LITE_HEBREW
	LITE_LANG_HEB,
	#endif

	#ifdef LITE_ARABIC
	LITE_LANG_ARA,
	#endif

	#ifdef LITE_PERSIAN
	LITE_LANG_PER,
	#endif

	#ifdef LITE_THAI
	LITE_LANG_THAI,
	#endif

	LITE_LANG_COUNT,
}LiteLangType;


inline void lite_language_set(LiteLangType lang);
inline int lite_language_get();


#ifdef __cplusplus
}
#endif
#endif

