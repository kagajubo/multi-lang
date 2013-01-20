#ifndef __LITE_FONT_H__
#define __LITE_FONT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../lite.h"
#include "_font.h"
#include "unicode.h"
#include "mbcs.h"
#include "language/lite_language.h"

//#define DEBUG_LITE_FONT

#define FONT_ENCODE_ANSI		(1)
#define FONT_ENCODE_UTF8		(2)
#define FONT_ENCODE_UNICODE	(3)
#define DEFAULT_FONT_ENCODE	FONT_ENCODE_UTF8

#define ORIGINAL_FONT_SIZE	(24)


//////////////////////////////////////////////////////////////////////////
//
typedef Font_t LiteFont;

typedef struct{
	LiteU8 width;
	LiteU8 size;
	LiteU8 *pdata;
}LiteCharInfo;

int lite_font_init(LiteLangType lang);
int lite_font_destroy();

int lite_default_font_size();
int lite_string_width(LiteChr *str);
int lite_char(LiteU16 unicode, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteColor color);
int lite_string(LiteChr *string, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteU16 w,LiteU16 h,LiteColor color);


#ifdef __cplusplus
}
#endif
#endif
