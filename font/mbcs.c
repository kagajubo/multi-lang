/***********************************************************
描述：	用c语言写的一个如何从MBCS编码格式的点阵字库中读取字符信息（像素宽 +点阵信息）
        至于容错性和效率方面还得使用者自行改善。谢谢您的参阅！
作者：  wujianguo
日期： 	20090516
MSN:    wujianguo19@hotmail.com
qq：    9599598
*************************************************************/
#include "mbcs.h"


#define		IS_CJK(codepage)		(codepage & 0xf)   //判断是否为中日韩字符集。如是则默认为字符等宽。
#define		SET_BIT(n)				(1<<n)

enum{
	CP932 = SET_BIT(0),	//Japanese
	CP936 = SET_BIT(1),	//s_chinese
	CP949 = SET_BIT(2),	//korea
	CP950 = SET_BIT(3),	//t-chinese
	CP874 = SET_BIT(4),
	CP1250 = SET_BIT(5),
	CP1251 = SET_BIT(6),
	CP1252 = SET_BIT(7),
	CP1253 = SET_BIT(8),
	CP1254 = SET_BIT(9),
	CP1255 = SET_BIT(10),
	CP1256 = SET_BIT(11),
	CP1257 = SET_BIT(12),
	CP1258 = SET_BIT(13),
};

static long GetPosWithMbcs(LiteU16 wCode, int nCodepage)
{
	long lIdx = -1;

	LiteU8 R = (wCode >> 8) & 0xFF;   //区码
	LiteU8 C = wCode & 0xFF;   //位码

	switch(nCodepage)
	{
	case CP932:
		if(R >= 0x81 && R <= 0x9F)
		{
			if(C >= 0x40 && C <= 0x7E)
				lIdx = (R-0x81)*188 + (C-0x40);  //188 = (0x7E-0x40+1)+(0xFC-0x80+1); 			
			else if(C >= 0x80 && C <= 0xFC)
				lIdx = (R-0x81)*188 + (C-0x80)+63;  // 63 = 0x7E-0x40+1;			
		}
		else if(R >= 0xE0 && R <= 0xFC)
		{
			if(C >= 0x40 && C <= 0x7E)
				lIdx = 5828 + (R-0xE0)*188 + (C-0x40);  // 5828 = 188 * (0x9F-0x81+1);
			else if(C >= 0x80 && C <= 0xFC)
				lIdx = 5828 + (R-0xE0)*188 + (C-0x80)+63;
		}
		break;

	case CP936:
		if((R >= 0xA1 && R <= 0xFE) && (C >= 0xA1 && C <= 0xFE))
			lIdx = (R-0xa1)*94 + (C-0xa1);  //94 = (0xFE-0xA1+1); 
		break;

	case CP949:
		if(R >= 0x81)
		{
			if(C >= 0x41 && C <= 0x7E)
				lIdx = ((R-0x81) * 188 + (C - 0x41));   // 188 = (0x7E-0x41+1)+(0xFE-0x81+1);
			else if(C >= 0x81 && C <= 0xFE)
				lIdx = ((R-0x81) * 188 + (C - 0x81) + 62);  // 62 = (0x7E-0x41+1);
		}
		break;

	case CP950:
		if(R >= 0xA1 && R <= 0xFE) 
		{
			if(C >= 0x40 && C <= 0x7E)
				lIdx = ((R-0xa1)*157+(C-0x40));   // 157 = (0x7E-0x40+1)+(0xFE-0xA1+1);
			else if(C >= 0xA1 && C <= 0xFE)
				lIdx = ((R-0xa1)*157+(C-0xa1)+63);  // 63 = (0x7E-0x40+1);
		}
		break;
	default:
		break;
	}
	return lIdx;
}

// 获取字符的像素宽度
LiteU32 ReadCharDistX_M(Font_t *pfont,LiteU16 wCode)
{
	PFL_HEADER pfl_header = pfont->header;
	LiteU32 offset = 0;
	memset(&pfont->dwCharInfo,0,sizeof(UFL_CHAR_INDEX));

	if(!pfl_header->nSection)  // CJK (等宽字符). 文件格式：文件头+点阵数据。
	{
		long lIdx = GetPosWithMbcs(wCode, pfl_header->wCpFlag);
		if(lIdx != -1) 
		{
#ifdef _BIG_ENDIAN
			pfont->dwCharInfo.be_format.OffAddr = sizeof(FL_HEADER) + lIdx * ((pfl_header->YSize + 7)/PIXELS_PER_BYTE* pfl_header->YSize);
			pfont->dwCharInfo.be_format.Width = pfl_header->YSize;
#else
			pfont->dwCharInfo.le_format.OffAddr = sizeof(FL_HEADER) + lIdx * ((pfl_header->YSize + 7)/PIXELS_PER_BYTE* pfl_header->YSize);
			pfont->dwCharInfo.le_format.Width = pfl_header->YSize;
#endif
			return pfl_header->YSize;
		}
	}
	else   // 拉丁文 (非等宽字符).   文件格式： 文件头+索引表+点阵数据。
	{	
		if(wCode <= 0xff)
		{		
			offset = sizeof(FL_HEADER) + wCode * FONT_INDEX_TAB_SIZE;
			memcpy(&(pfont->dwCharInfo),(LiteU8 *)pfont->header+offset,sizeof(UFL_CHAR_INDEX));
#ifdef _BIG_ENDIAN
			//printf("be :offset : %#10x , width:%d",pfont->dwCharInfo.be_format.OffAddr,pfont->dwCharInfo.be_format.Width);
			return pfont->dwCharInfo.be_format.Width;
#else
			//printf("le :offset : %#10x , width:%d",pfont->dwCharInfo.le_format.OffAddr,pfont->dwCharInfo.le_format.Width);
			return pfont->dwCharInfo.le_format.Width;
#endif
		}
	}
	return 0;
}
