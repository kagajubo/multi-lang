#ifndef __1004A_H__
#define __1004A_H__

#include "common.h"

#define MAX_CODES		1024
#define CODE_SIZE		2
#define DEFAULT_FONT_HEIGHT	24

#define DEFAULT_FONT_FILE_NAME	"font_XXx24_%s.h"
#define GET_FONT_FILE_NAME(name,lang) (sprintf(name,DEFAULT_FONT_FILE_NAME,lang))	

#define DEFAULT_FONT_TABLE_FILE_NAME		"font_table_%s.h"
#define DEFAULT_FONT_TABLE_ARRAY_NAME		"font_unicode_table_%s"
#define GET_FONT_TABLE_FILE_NAME(name,lang)	(sprintf(name,DEFAULT_FONT_TABLE_FILE_NAME,lang))
#define GET_FONT_TABLE_ARRAY_NAME(name,lang)	(sprintf(name,DEFAULT_FONT_TABLE_ARRAY_NAME,lang))


#define DEFAULT_FONT_WIDTH_FILE_NAME		"font_width_table_%s.h"
#define DEFAULT_FONT_WIDTH_ARRAY_NAME		"font_width_table_%s"
#define GET_FONT_WIDTH_FILE_NAME(name,lang)	(sprintf(name,DEFAULT_FONT_WIDTH_FILE_NAME,lang))
#define GET_FONT_WIDTH_ARRAY_NAME(name,lang)	(sprintf(name,DEFAULT_FONT_WIDTH_ARRAY_NAME,lang))


//if pCodeFile==NULL, use startCode and  endCode ; else use codefile
int  _1004A_CArray(unsigned short startCode,unsigned short endCode,char *pCodeFile,char *lang);

#endif
