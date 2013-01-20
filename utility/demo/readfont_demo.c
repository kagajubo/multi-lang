#include "font\font.h"

#define	MAX_FONT_SIZE					64
#define	MODE_DEBUG						0
 

extern void DisplayChar(int nYSize, int nBytesPerLine, unsigned char *pFontData);

void HowToUser()
{	
	printf("*********************************************************************\n");
	printf("Example:\n");
	printf("\tReadfontInfo.exe D:\\test\\a\\aa\\arialuni_U16.bin 33\n\n");

	printf("QQ:	9599598\n");
	printf("Msn:	wujianguo19@hotmail.com\n");
	printf("*********************************************************************/\n\n");
}




void main(int argc, char *argv[])
{
#if(!MODE_DEBUG)
	if(argc != 3)
	{
		HowToUser();
	}
	else
#endif
	{
#if(MODE_DEBUG)
		char *pfontfile = "d:\\test\\a\\aa\\arial24_CP1252.bin";//"D:\\test\\a\\aa\\arialuni_U16.bin";
		WORD wCode = 0x21;
#else
		char *pfontfile = argv[1];  
		WORD wCode = atoi(argv[2]); 
#endif

		if(InitFont(pfontfile))
		{
			int i = 0, j = 0;
			int nWidth = 0;
			WORD bytesPerLine = 0;
			BYTE buf[MAX_FONT_SIZE * (MAX_FONT_SIZE/PIXELS_PER_BYTE)];
			
			nWidth = ReadCharDistX(wCode);
			printf("wCode = 0x%x\n",  wCode);
			printf("nWidth = %d\n", nWidth);
			if(ReadCharDotArray(wCode, buf, &bytesPerLine))
				DisplayChar(GetFontYSize(), bytesPerLine, buf);

			ExitFont();
		}	
	}
}