#include "lite_language.h"


#include "strtable_eng.c"
#if defined(LITE_S_CHINESE)
#include "strtable_sch.c"
#endif
#if defined(LITE_RUSSIAN)
#include "strtable_rus.c"
#endif
#if defined(LITE_KOREAN)
#include "strtable_kor.c"
#endif



int lite_lang=LITE_LANG_SCH;

const char * const *g_StrTable[] = 
{
	str_table_eng,
#if defined(LITE_S_CHINESE)
	str_table_chi_simplified,
#endif
#if defined(LITE_RUSSIAN)
	str_table_rus,
#endif
#if defined(LITE_KOREAN)
	str_table_kor,
#endif
};

inline void lite_language_set(LiteLangType lang)
{
	lite_lang=lang;
}

inline int lite_language_get()
{
	return lite_lang;
}

