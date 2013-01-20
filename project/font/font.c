/***********************************************************
描述：	用c语言写的一个如何从点阵字库中读取字符信息（像素宽 +点阵信息）
        至于容错性和效率方面还得使用者自行改善,谢谢您的参阅！

文件格式： 
		Unicode -- 文件头+数据段(section)+索引表+点阵信息
		MBCS_Ladin-文件头+索引表+点阵信息
		MBCS_CJK-- 文件头+点阵信息
		
作者：  wujianguo
日期： 	20090516
MSN:    wujianguo19@hotmail.com
qq：    9599598
*************************************************************/

#include "font.h"
#include "unicode.h"
#include "Mbcs.h"

FILE *g_prf = NULL;
FL_HEADER _fl_header;
DWORD g_dwCharInfo = 0;    // 存储当前字符的检索信息。  bit0～bit25：存放点阵信息的起始地址。 bit26～bit31：存放像素宽度。

static int ReadFontHeader(PFL_HEADER pfl_header)
{	
	fread(pfl_header, sizeof(FL_HEADER) -4, 1, g_prf);

	//检测表示头
	if((_fl_header.magic[0] != 'U' && _fl_header.magic[0] != 'M')
		|| _fl_header.magic[1] != 'F' || _fl_header.magic[2] != 'L')
	{
		printf("Cann't support file format!\n");
		return 0;
	}

	if('U' == pfl_header->magic[0])     //unicode 编码
	{
		return ReadFontSection();
	}

	return 1;	
}

static int OpenFontFile(char *pFontFile)
{
	if((g_prf = fopen(pFontFile, "rb")) == NULL)
	{
		printf("Cann't open : %s\n", pFontFile);
		return 0;
	}
		
	return 1;
}

int PrintFontInfo(char *pFontfile)
{
	int i;
	printf("*******************************font:%s ********************\r\n",pFontfile);
	printf("* font encode type: %s \r\n",_fl_header.magic[0] == 'M' ? "MBCS" : "UNICODE");
	if(_fl_header.magic[0]=='U')
	{
		printf("********* font section info, total section num : %d \r\n",_fl_header.nSection);
		for(i=0;i<_fl_header.nSection;i++)
		{
			printf("*\t section %d: start font:%04x, end font:%04x, offset:%08x \r\n",
				i,_fl_header.pSection[i].First,_fl_header.pSection[i].Last,_fl_header.pSection[i].OffAddr);
		}
	}
	printf("*****************************************************************\r\n");
	return 0;
}

/***************************************************************
功能： 初始化字体。 即打开字体文件，且读取信息头。
参数： pFontFile--字库文件名      
***************************************************************/
int InitFont(char *pFontFile)
{
	memset(&_fl_header, 0, sizeof(FL_HEADER));

	if(OpenFontFile(pFontFile))
		return ReadFontHeader(&_fl_header);	
	else
		return 0;
}

void ExitFont()
{
	if('U' == _fl_header.magic[0])     //unicode 编码
		ReleaseSection();
}

BYTE GetFontYSize()
{
	return _fl_header.YSize;
}


/********************************************************************
功能： 获取当前字符的像素宽度, 且将索引信息存入一个全局变量：g_dwCharInfo。
        根据索引信息，即同时能获取当前字符的点阵信息的起始地址。
参数： wCode -- 当字库为unicode编码格式时，则将wCode当unicode编码处理。
                否则反之（MBCS)。
********************************************************************/
int ReadCharDistX(WORD wCode)
{
	if('U' == _fl_header.magic[0])     //unicode 编码
		return ReadCharDistX_U(wCode);
	else
		return ReadCharDistX_M(wCode);
}

/**********************************************************************
功能： 获取点阵信息
参数： wCode 在这里预留，主要是因为前面有保存一个全局的g_dwCharInfo，也就知道了该字符的相应信息(宽度+点阵信息的起始地址)。
       fontArray 存放点阵信息
	   bytesPerLine 每一行占多少个字节。
**********************************************************************/
int ReadCharDotArray(WORD wCode, BYTE *fontArray, WORD *bytesPerLine)
{	
	*bytesPerLine= (WORD)((GET_FONT_WIDTH(g_dwCharInfo))+7)/PIXELS_PER_BYTE;	

	if(g_dwCharInfo > 0)
	{		
		DWORD nDataLen = *bytesPerLine * _fl_header.YSize;			
		DWORD  dwOffset = GET_FONT_OFFADDR(g_dwCharInfo);    //获取字符点阵的地址信息(低26位)
		
		fseek(g_prf, dwOffset, SEEK_SET);
		fread(fontArray, nDataLen, 1, g_prf);	
		
		return 1;
	}
	
	return 0;
}

int GetRealCX(int nYSize, int nBytesPerLine, unsigned char *pFontData)
{
	int i, j, k;
	int real_cx;
	//printf("bytesPerLine = %d\n", nBytesPerLine);
	
	for(i = 0; i < nYSize; i++)	
	{
		for(j = 0; j < nBytesPerLine; j++)
		{
			unsigned char tmp=pFontData[i * nBytesPerLine + j];
			for(k=0;k<8;k++)
			{
				if(tmp & 0x80)
				{
					real_cx=j*8+k+1;
					tmp <<=1;
				}
					
			}
		}
	}
	printf("real cx:%d \r\n",real_cx);
	return real_cx;
}