#ifndef __LITE_STRING_TABLE_H__
#define __LITE_STRING_TABLE_H__

#include "lite_language.h"

extern const char * const *g_StrTable[];
extern int lite_lang;


#define LITE_STRING(id)	(g_StrTable[lite_lang][id%LITE_IDS_END])

enum {
	LITE_IDS_NULL = 0				,
	LITE_IDS_TEST1				,
	LITE_IDS_TEST2			,
	LITE_IDS_TEST3				,
	LITE_IDS_TEST4			,
	LITE_IDS_TEST5				,
	LITE_IDS_TEST6			,
	LITE_IDS_TEST7				,
	LITE_IDS_TEST8			,
	LITE_IDS_TEST9				,
	LITE_IDS_TEST10			,
	
	LITE_IDS_END
};

#endif

