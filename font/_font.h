#ifndef __L_FONT_H__
#define __L_FONT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "../lite_def.h"

//#define USE_WHOLE_CODEPAGE	TRUE	// TRUE OR FALSE
#define WHOLE_CODEPAGE_CONF_INDEX	(1)
#define USE_WHOLE_CODEPAGE_FLAG		('M')
#define USE_PARTIAL_CODEPAGE_FLAG	('P')

#ifdef LINUX
#define _LITTLE_ENDIAN	
//#define CONVERT_TO_LITTLE_ENDIAN
#else
#define _BIG_ENDIAN
//#define CONVERT_TO_BIG_ENDIAN
#endif

#define font_swab32(x) ((x&0x000000ff) << 24 | (x&0x0000ff00) << 8 | (x&0x00ff0000) >> 8 | (x&0xff000000) >> 24)
#define font_swab16(x) ((x&0x00ff) << 8 | (x&0xff00) >> 8)


#define PIXELS_PER_BYTE					8
#define	FONT_INDEX_TAB_SIZE				4    //单个字符对应的字体检索信息长度为 4Byte  (b0~b25: 记录点阵信息的起始地址, b26~b31: 记录当前字符的象素宽度)
#define	GET_FONT_WIDTH(charinfo)		(charinfo >> 26)
#define	GET_FONT_OFFADDR(charinfo)		(charinfo & 0x3ffffff)
#define	DB2UC(c1,c2)					(c1 | (((U16)c2)<<8))

#pragma pack(1)

typedef struct tagFlSectionInfo{
	LiteU16  First;        
	LiteU16  Last;         
	LiteU32 OffAddr;     
} FL_SECTION_INF, *PFL_SECTION_INF;

typedef struct tagFontLibHeader{
	LiteU8    magic[4];    //'U'(or ’M’), 'F', 'L', X---Unicode(or MBCS) Font Library, X: 表示版本号. 分高低4位。如 0x12表示 Ver 1.2
	LiteU32 	Size;            
	LiteU8    nSection; // 共分几段数据，主要针对 UNICODE 编码有效。
	LiteU8    YSize;                 
	LiteU16    wCpFlag;    // codepageflag:  bit0~bit13 每个bit分别代表一个CodePage 标志，如果是1，则表示当前CodePage 被选定，否则为非选定。
	char    reserved[4];      // 预留字节    
} FL_HEADER, *PFL_HEADER;

typedef union _ufl_char_index{
	LiteU32 padding;

	struct{
		LiteU32  OffAddr	: 26;	   // 当前字符点阵数据的起始地址
		LiteU32  Width	: 6;	   // 字符点阵的像素的宽度( 目前最大支持 56 点阵）
	}le_format;
	struct{
		LiteU32	 Width	: 6;
		LiteU32	 OffAddr	: 26;
	}be_format;
}UFL_CHAR_INDEX;

typedef struct _ufl_char_index_ex{	
	union{
		LiteU32 padding;
		
		struct {
			LiteU32  OffAddr	: 26;
			LiteU32  Width	: 6;	
		}le_format;
		struct _big_partial_format{
			LiteU32	 Width	: 6;
			LiteU32	 OffAddr	: 26;
		}be_format;
	};
	LiteU16 Code;
	LiteU16	reserved;
}UFL_CHAR_INDEX_EX;


typedef struct __font{
	LiteU8 lang;
	FL_HEADER *header;
	FL_SECTION_INF	**pSection;
	union{
		UFL_CHAR_INDEX dwCharInfo;
		UFL_CHAR_INDEX_EX dwCharInfoEx;
	};
}Font_t;

#pragma pack()

void print_font_info(Font_t *pfont);
int init_font(Font_t *pfont);
int destroy_font(Font_t *pfont);



#ifdef __cplusplus
}
#endif
#endif

