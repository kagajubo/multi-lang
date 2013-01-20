#include "stdio.h"
#include "stdlib.h"
#include "Common.h"
#include "BmpRW.h"

/*
Copyright:	Li Weiwei
VER:		2.00
Function:	读取一幅256灰度的bmp图片入内存
Input:		bmp文件名指针，内存中图片数据指针
Output:		无
*/
void GrayBmpR(char *fn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;			// bmp文件头指针
	BITMAPINFOHEADER *pbmpi;			// bmp文件信息头指针
	unsigned char    *pbitstr;			// bmp文件数据指针
	int				 i = 0;

	// 将bmp文件头文件读入*pbmpfh
	FILE *rfile = fopen(fn,"rb+");
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,sizeof(BITMAPFILEHEADER),1,rfile);
	
	// 将bmp文件信息头文件读入*pbmpi
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,sizeof(BITMAPINFOHEADER),1,rfile);
	
	// 将bmp文件数据读入*pbitstr
	pbitstr = (unsigned char*) malloc(IMAGE_SIZE);
	if (!pbitstr)
	{
		printf("pbitstr not enough memory");
	}
	fseek(rfile,pbmpfh->bfOffBits,SEEK_SET);
	fread(pbitstr,IMAGE_SIZE,1,rfile);
	fclose(rfile);				

	// 将*pbitstr中数据存入*pImage
	for (i = 0; i < IMAGE_SIZE; i++)
	{
		*(pImage+i) = *(pbitstr+i);
	}

	// 释放不用的内存空间
	free(pbitstr);
	free(pbmpi);  
	free(pbmpfh);
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	读取一幅24位真彩色bmp图片入内存
Input:		bmp文件名指针，内存中图片数据指针
Output:		无
*/
void ColorBmpR(char *fn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;			// bmp文件头指针
	BITMAPINFOHEADER *pbmpi;			// bmp文件信息头指针
	unsigned char    *pbitstr;			// bmp文件数据指针
	int							 i = 0;

	// 将bmp文件头文件读入*pbmpfh
	FILE *rfile = fopen(fn,"rb+");
	pbmpfh = (BITMAPFILEHEADER*) calloc(1, sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,sizeof(BITMAPFILEHEADER),1,rfile);
	
	// 将bmp文件信息头文件读入*pbmpi
	pbmpi = (BITMAPINFOHEADER*) calloc(1, sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,sizeof(BITMAPINFOHEADER),1,rfile);

	// 将bmp文件数据读入*pbitstr
	pbitstr = (unsigned char*) calloc(IMAGE_SIZE*3, sizeof(char));
	if (!pbitstr)
	{
		printf("pbitstr not enough memory");
	}
	fseek(rfile,pbmpfh->bfOffBits,SEEK_SET);
	fread(pbitstr,IMAGE_SIZE*3,1,rfile);
	fclose(rfile);
	
	// 将*pbitstr中数据存入*pImage
	for( i=0 ; i< IMAGE_SIZE*3 ; i++ )
	{
		*(pImage+i) = *(pbitstr+i);	
	}

	// 释放不用的内存空间
	free(pbitstr);
	free(pbmpi);  
	free(pbmpfh);  
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	写一幅256灰度的bmp图片入文件
Input:		灰度模板文件名指针，待写入bmp文件名指针，内存中图片数据指针
Output:		无
*/
void GrayBmpW(char *rfn, char *wfn, unsigned char *pImage)
{
	BITMAPINFOHEADER *pbmpi;				// bmp文件信息头指针
	BITMAPFILEHEADER *pbmpfh;				// bmp文件头指针
	RGBQUAD			 *pbmprgb;				// bmp文件调色板指针
	unsigned char	 *pbitstr;				// bmp文件数据指针
	short			 bmp_header   = 0x4D42;	// bmp文件头字
	short			 bmp_end      = 0x0000;	// bmp文件尾字
	int				 i = 0;

	FILE *rfile = fopen(rfn,"rb+");			// 模板bmp文件指针
	FILE *wfile = fopen(wfn,"wb+");			// 生成bmp文件指针
		
	// 将bmp头字写入文件
	fseek(wfile,0,SEEK_SET);
	fwrite(&bmp_header,1,sizeof(short),wfile);

	// 将模板bmp文件头文件写入生成文件
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,1,sizeof(BITMAPFILEHEADER),rfile);
	fseek(wfile,sizeof(short),SEEK_SET);
	fwrite(pbmpfh,1,sizeof(BITMAPFILEHEADER),wfile);
	
	// 将模板bmp文件信息头文件写入生成文件
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,1,sizeof(BITMAPINFOHEADER),rfile);
	fseek(wfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmpi,1,sizeof(BITMAPINFOHEADER),wfile);
	 
	// 将模板bmp调色板写入生成文件
	pbmprgb = (RGBQUAD*) calloc(256,sizeof(RGBQUAD));
	if (!pbmprgb)
	{
		printf("pbmprgb not enough memory");
	}
	fseek(rfile,sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmprgb,256,sizeof(RGBQUAD),rfile);
	fclose(rfile);	// 关闭模板文件
	fseek(wfile,sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmprgb,sizeof(RGBQUAD),256,wfile);
	
	// 将图像数据pImage写入生成文件
	fseek(wfile,pbmpfh->bfOffBits,SEEK_SET);
	pbitstr = (unsigned char*) malloc(IMAGE_SIZE);
	if (!pbitstr)
	{
		printf("pbitstr not enough memory");
	}
	for (i = 0; i < IMAGE_SIZE; i++)
	{
		*(pbitstr+i)=*(pImage+i);
	}
	fwrite(pbitstr,IMAGE_SIZE,1,wfile);
	free(pbitstr);
	
	// 将bmp尾字写入文件
	fseek(wfile,0,SEEK_END);
	fwrite(&bmp_end,1,sizeof(short),wfile);
	fclose(wfile);	// 关闭生成文件

	// 释放不用的内存空间
	free(pbmprgb); 
	free(pbmpi);
	free(pbmpfh);
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	写一幅24位真彩色bmp图片入文件
Input:		24位真彩色模板文件名指针，待写入bmp文件名指针，内存中图片数据指针
Output:		无
*/
void ColorBmpW(char *rfn, char *wfn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;				// bmp文件头指针
	BITMAPINFOHEADER *pbmpi;				// bmp文件信息头指针
	unsigned char	 *pbitstr;				// bmp文件数据指针
	short			 bmp_header   = 0x4D42;	// bmp文件头字
	int				 i = 0;

	FILE *rfile = fopen(rfn,"rb+");			// 模板bmp文件指针
	FILE *wfile = fopen(wfn,"wb+");			// 生成bmp文件指针

	// 将bmp头字写入文件
	fseek(wfile, 0, SEEK_SET);
    fwrite(&bmp_header,1,sizeof(short),wfile);
		
	// 将模板bmp文件头文件写入生成文件
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,1,sizeof(BITMAPFILEHEADER),rfile);
	fseek(rfile,sizeof(short),SEEK_SET);
	fwrite(pbmpfh,1,sizeof(BITMAPFILEHEADER),wfile);
	
	// 将模板bmp文件信息头文件写入生成文件
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,1,sizeof(BITMAPINFOHEADER),rfile);
	fseek(wfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmpi,1,sizeof(BITMAPINFOHEADER),wfile);
	 
	// 将图像数据写入生成文件
	pbitstr = (unsigned char*) calloc(IMAGE_SIZE*3,sizeof(char));
	if (!pbitstr)
	{
		printf("not enough memory");
	}
	
	// 将*pImage中数据写入*pbitstr
	for( i=0 ; i< IMAGE_SIZE*3 ; i++ )
	{
  	*(pbitstr+i) = *(pImage+i);
  }

	fseek(wfile,pbmpfh->bfOffBits,SEEK_SET);
	fwrite(pbitstr,IMAGE_SIZE*3,1,wfile);
	fclose(wfile);

	// 释放不用的内存空间
	free(pbitstr);
	free(pbmpi);
	free(pbmpfh);
}
