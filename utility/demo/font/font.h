// font.h 
//

#if !defined(AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
#define AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "..\include\typedef.h"

#define PIXELS_PER_BYTE					8
#define	FONT_INDEX_TAB_SIZE				4    //�����ַ���Ӧ�����������Ϣ����Ϊ 4Byte  (b0~b25: ��¼������Ϣ����ʼ��ַ, b26~b31: ��¼��ǰ�ַ������ؿ��)
#define	GET_FONT_WIDTH(charinfo)		(charinfo >> 26)
#define	GET_FONT_OFFADDR(charinfo)		(charinfo & 0x3ffffff)
#define	DB2UC(c1,c2)					(c1 | (((U16)c2)<<8))

typedef struct tagFlSectionInfo{
            WORD  First;        
            WORD  Last;         
            DWORD OffAddr;     
       } FL_SECTION_INF, *PFL_SECTION_INF;

typedef struct tagFontLibHeader{
            BYTE    magic[4];    //'U'(or ��M��), 'F', 'L', X---Unicode(or MBCS) Font Library, X: ��ʾ�汾��. �ָߵ�4λ���� 0x12��ʾ Ver 1.2
            DWORD 	Size;            
            BYTE    nSection; // ���ּ������ݣ���Ҫ��� UNICODE ������Ч��
            BYTE    YSize;                 
            WORD    wCpFlag;    // codepageflag:  bit0~bit13 ÿ��bit�ֱ����һ��CodePage ��־�������1�����ʾ��ǰCodePage ��ѡ��������Ϊ��ѡ����
            char    reserved[4];      // Ԥ���ֽ�    
            FL_SECTION_INF  *pSection;
        } FL_HEADER, *PFL_HEADER;


int  InitFont(char *pFontFile);
void ExitFont();

BYTE GetFontYSize();
int  ReadCharDistX(WORD wCode);
int  ReadCharDotArray(WORD wCode, BYTE *fontArray, WORD *bytesPerLine);
//int  OpenFontFile(char *pFontFile);
//void ReadFontHeader(PFL_HEADER pfl_header);
//DWORD ReadCharDistX(WORD wCode);
//int ReadCharDotArray(WORD wCode, BYTE *fontArray, WORD *bytesPerLine);

#endif // !defined(AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
