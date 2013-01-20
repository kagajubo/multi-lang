#include "stdio.h"
#include "stdlib.h"
#include "Common.h"
#include "BmpRW.h"

/*
Copyright:	Li Weiwei
VER:		2.00
Function:	��ȡһ��256�Ҷȵ�bmpͼƬ���ڴ�
Input:		bmp�ļ���ָ�룬�ڴ���ͼƬ����ָ��
Output:		��
*/
void GrayBmpR(char *fn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;			// bmp�ļ�ͷָ��
	BITMAPINFOHEADER *pbmpi;			// bmp�ļ���Ϣͷָ��
	unsigned char    *pbitstr;			// bmp�ļ�����ָ��
	int				 i = 0;

	// ��bmp�ļ�ͷ�ļ�����*pbmpfh
	FILE *rfile = fopen(fn,"rb+");
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,sizeof(BITMAPFILEHEADER),1,rfile);
	
	// ��bmp�ļ���Ϣͷ�ļ�����*pbmpi
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,sizeof(BITMAPINFOHEADER),1,rfile);
	
	// ��bmp�ļ����ݶ���*pbitstr
	pbitstr = (unsigned char*) malloc(IMAGE_SIZE);
	if (!pbitstr)
	{
		printf("pbitstr not enough memory");
	}
	fseek(rfile,pbmpfh->bfOffBits,SEEK_SET);
	fread(pbitstr,IMAGE_SIZE,1,rfile);
	fclose(rfile);				

	// ��*pbitstr�����ݴ���*pImage
	for (i = 0; i < IMAGE_SIZE; i++)
	{
		*(pImage+i) = *(pbitstr+i);
	}

	// �ͷŲ��õ��ڴ�ռ�
	free(pbitstr);
	free(pbmpi);  
	free(pbmpfh);
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	��ȡһ��24λ���ɫbmpͼƬ���ڴ�
Input:		bmp�ļ���ָ�룬�ڴ���ͼƬ����ָ��
Output:		��
*/
void ColorBmpR(char *fn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;			// bmp�ļ�ͷָ��
	BITMAPINFOHEADER *pbmpi;			// bmp�ļ���Ϣͷָ��
	unsigned char    *pbitstr;			// bmp�ļ�����ָ��
	int							 i = 0;

	// ��bmp�ļ�ͷ�ļ�����*pbmpfh
	FILE *rfile = fopen(fn,"rb+");
	pbmpfh = (BITMAPFILEHEADER*) calloc(1, sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,sizeof(BITMAPFILEHEADER),1,rfile);
	
	// ��bmp�ļ���Ϣͷ�ļ�����*pbmpi
	pbmpi = (BITMAPINFOHEADER*) calloc(1, sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,sizeof(BITMAPINFOHEADER),1,rfile);

	// ��bmp�ļ����ݶ���*pbitstr
	pbitstr = (unsigned char*) calloc(IMAGE_SIZE*3, sizeof(char));
	if (!pbitstr)
	{
		printf("pbitstr not enough memory");
	}
	fseek(rfile,pbmpfh->bfOffBits,SEEK_SET);
	fread(pbitstr,IMAGE_SIZE*3,1,rfile);
	fclose(rfile);
	
	// ��*pbitstr�����ݴ���*pImage
	for( i=0 ; i< IMAGE_SIZE*3 ; i++ )
	{
		*(pImage+i) = *(pbitstr+i);	
	}

	// �ͷŲ��õ��ڴ�ռ�
	free(pbitstr);
	free(pbmpi);  
	free(pbmpfh);  
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	дһ��256�Ҷȵ�bmpͼƬ���ļ�
Input:		�Ҷ�ģ���ļ���ָ�룬��д��bmp�ļ���ָ�룬�ڴ���ͼƬ����ָ��
Output:		��
*/
void GrayBmpW(char *rfn, char *wfn, unsigned char *pImage)
{
	BITMAPINFOHEADER *pbmpi;				// bmp�ļ���Ϣͷָ��
	BITMAPFILEHEADER *pbmpfh;				// bmp�ļ�ͷָ��
	RGBQUAD			 *pbmprgb;				// bmp�ļ���ɫ��ָ��
	unsigned char	 *pbitstr;				// bmp�ļ�����ָ��
	short			 bmp_header   = 0x4D42;	// bmp�ļ�ͷ��
	short			 bmp_end      = 0x0000;	// bmp�ļ�β��
	int				 i = 0;

	FILE *rfile = fopen(rfn,"rb+");			// ģ��bmp�ļ�ָ��
	FILE *wfile = fopen(wfn,"wb+");			// ����bmp�ļ�ָ��
		
	// ��bmpͷ��д���ļ�
	fseek(wfile,0,SEEK_SET);
	fwrite(&bmp_header,1,sizeof(short),wfile);

	// ��ģ��bmp�ļ�ͷ�ļ�д�������ļ�
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("pbmpfh not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,1,sizeof(BITMAPFILEHEADER),rfile);
	fseek(wfile,sizeof(short),SEEK_SET);
	fwrite(pbmpfh,1,sizeof(BITMAPFILEHEADER),wfile);
	
	// ��ģ��bmp�ļ���Ϣͷ�ļ�д�������ļ�
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("pbmpi not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,1,sizeof(BITMAPINFOHEADER),rfile);
	fseek(wfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmpi,1,sizeof(BITMAPINFOHEADER),wfile);
	 
	// ��ģ��bmp��ɫ��д�������ļ�
	pbmprgb = (RGBQUAD*) calloc(256,sizeof(RGBQUAD));
	if (!pbmprgb)
	{
		printf("pbmprgb not enough memory");
	}
	fseek(rfile,sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmprgb,256,sizeof(RGBQUAD),rfile);
	fclose(rfile);	// �ر�ģ���ļ�
	fseek(wfile,sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmprgb,sizeof(RGBQUAD),256,wfile);
	
	// ��ͼ������pImageд�������ļ�
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
	
	// ��bmpβ��д���ļ�
	fseek(wfile,0,SEEK_END);
	fwrite(&bmp_end,1,sizeof(short),wfile);
	fclose(wfile);	// �ر������ļ�

	// �ͷŲ��õ��ڴ�ռ�
	free(pbmprgb); 
	free(pbmpi);
	free(pbmpfh);
}


/*
Copyright:	Li Weiwei
VER:		2.00
Function:	дһ��24λ���ɫbmpͼƬ���ļ�
Input:		24λ���ɫģ���ļ���ָ�룬��д��bmp�ļ���ָ�룬�ڴ���ͼƬ����ָ��
Output:		��
*/
void ColorBmpW(char *rfn, char *wfn, unsigned char *pImage)
{
	BITMAPFILEHEADER *pbmpfh;				// bmp�ļ�ͷָ��
	BITMAPINFOHEADER *pbmpi;				// bmp�ļ���Ϣͷָ��
	unsigned char	 *pbitstr;				// bmp�ļ�����ָ��
	short			 bmp_header   = 0x4D42;	// bmp�ļ�ͷ��
	int				 i = 0;

	FILE *rfile = fopen(rfn,"rb+");			// ģ��bmp�ļ�ָ��
	FILE *wfile = fopen(wfn,"wb+");			// ����bmp�ļ�ָ��

	// ��bmpͷ��д���ļ�
	fseek(wfile, 0, SEEK_SET);
    fwrite(&bmp_header,1,sizeof(short),wfile);
		
	// ��ģ��bmp�ļ�ͷ�ļ�д�������ļ�
	pbmpfh = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (!pbmpfh)
	{
		printf("not enough memory");
	}
	fseek(rfile,sizeof(short),SEEK_SET);
	fread(pbmpfh,1,sizeof(BITMAPFILEHEADER),rfile);
	fseek(rfile,sizeof(short),SEEK_SET);
	fwrite(pbmpfh,1,sizeof(BITMAPFILEHEADER),wfile);
	
	// ��ģ��bmp�ļ���Ϣͷ�ļ�д�������ļ�
	pbmpi = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (!pbmpi)
	{
		printf("not enough memory");
	}
	fseek(rfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fread(pbmpi,1,sizeof(BITMAPINFOHEADER),rfile);
	fseek(wfile,sizeof(BITMAPFILEHEADER)+sizeof(short),SEEK_SET);
	fwrite(pbmpi,1,sizeof(BITMAPINFOHEADER),wfile);
	 
	// ��ͼ������д�������ļ�
	pbitstr = (unsigned char*) calloc(IMAGE_SIZE*3,sizeof(char));
	if (!pbitstr)
	{
		printf("not enough memory");
	}
	
	// ��*pImage������д��*pbitstr
	for( i=0 ; i< IMAGE_SIZE*3 ; i++ )
	{
  	*(pbitstr+i) = *(pImage+i);
  }

	fseek(wfile,pbmpfh->bfOffBits,SEEK_SET);
	fwrite(pbitstr,IMAGE_SIZE*3,1,wfile);
	fclose(wfile);

	// �ͷŲ��õ��ڴ�ռ�
	free(pbitstr);
	free(pbmpi);
	free(pbmpfh);
}
