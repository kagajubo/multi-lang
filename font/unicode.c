/***********************************************************
描述：	用c语言写的一个如何从unicode编码格式的点阵字库中读取字符信息（像素宽 +点阵信息）
        至于容错性和效率方面还得使用者自行改善。谢谢您的参阅！
作者：  wujianguo
日期： 	20090516
MSN:    wujianguo19@hotmail.com
qq：    9599598
*************************************************************/

#include "unicode.h"



// 获取字符的像素宽度
LiteU32 ReadCharDistX_U(Font_t *pfont,LiteU16 wCode)
{	
	LiteU32 offset ;
	int   i;
	memset(&pfont->dwCharInfo,0,sizeof(UFL_CHAR_INDEX));
	
	for(i = 0;i<pfont->header->nSection;i++)
	{
		if(wCode >= pfont->pSection[i]->First && wCode <= pfont->pSection[i]->Last)
			break;		
	}
	if(i >= pfont->header->nSection)	
		return 0;	
	
	offset = pfont->pSection[i]->OffAddr+ FONT_INDEX_TAB_SIZE*(wCode - pfont->pSection[i]->First );	
	pfont->dwCharInfo=*((UFL_CHAR_INDEX *)((LiteU8 *)pfont->header+offset));
	
#ifdef _BIG_ENDIAN
	//printf("be :offset : %#10x , width:%d",pfont->dwCharInfo.be_format.OffAddr,pfont->dwCharInfo.be_format.Width);
	return pfont->dwCharInfo.be_format.Width;
#else
	//printf("le :offset : %#10x , width:%d",pfont->dwCharInfo.le_format.OffAddr,pfont->dwCharInfo.le_format.Width);
	return pfont->dwCharInfo.le_format.Width;
#endif
}

LiteU32 ReadCharDistX_UP(Font_t *pfont,LiteU16 wCode)
{	
	memset(&pfont->dwCharInfoEx,0,sizeof(UFL_CHAR_INDEX_EX));
	UFL_CHAR_INDEX_EX *pcharInfo=(UFL_CHAR_INDEX_EX *)((LiteU8 *)pfont->header+sizeof(FL_HEADER));
	//printf("nsectin : %d \n",pfont->header->nSection);
	for(;pcharInfo->Code;pcharInfo++)
	{
		//printf("pcharinfo : %#10x,code:%#06x,width:%d,off:%d\n",pcharInfo,
		//	pcharInfo->Code,pcharInfo->be_format.Width,pcharInfo->be_format.OffAddr);
		if(pcharInfo->Code == wCode)
			break;
	}
	if(pcharInfo->Code != wCode){	
		printf("ERROR: not found code: %#06x \n",wCode);
		return 0;	
	}
	
	//pfont->dwCharInfoEx=*(pcharInfo);
	memcpy(&pfont->dwCharInfoEx,pcharInfo,sizeof(UFL_CHAR_INDEX_EX));
	
#ifdef _BIG_ENDIAN
	//printf("be :offset : %#10x , width:%d \n",pfont->dwCharInfoEx.be_format.OffAddr,pfont->dwCharInfoEx.be_format.Width);
	return pfont->dwCharInfoEx.be_format.Width;
#else
	//printf("le :offset : %#10x , width:%d \n",pfont->dwCharInfoEx.le_format.OffAddr,pfont->dwCharInfoEx.le_format.Width);
	return pfont->dwCharInfoEx.le_format.Width;
#endif
}

