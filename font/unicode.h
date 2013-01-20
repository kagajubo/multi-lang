#ifndef __UNICODE_H__
#define __UNICODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "_font.h"

LiteU32 ReadCharDistX_U(Font_t *pfont,LiteU16 wCode);
LiteU32 ReadCharDistX_UP(Font_t *pfont,LiteU16 wCode);

#ifdef __cplusplus
}
#endif
#endif

