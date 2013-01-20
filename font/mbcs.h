#ifndef __MBCS_H__
#define __MBCS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "_font.h"

LiteU32 ReadCharDistX_M(Font_t *pfont,LiteU16 wCode);

#ifdef __cplusplus
}
#endif
#endif
