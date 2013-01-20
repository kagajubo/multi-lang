/***********************************************************
������	��c����д��һ����δ�unicode�����ʽ�ĵ����ֿ��ж�ȡ�ַ���Ϣ�����ؿ� +������Ϣ��
        �����ݴ��Ժ�Ч�ʷ��滹��ʹ�������и��ơ�лл���Ĳ��ģ�
���ߣ�  wujianguo
���ڣ� 	20090516
MSN:    wujianguo19@hotmail.com
qq��    9599598
*************************************************************/

#include "..\font.h"
#include "unicode.h"


        
extern FILE *g_prf;
extern FL_HEADER _fl_header;      
extern DWORD g_dwCharInfo;   

int ReadFontSection()
{
	PFL_HEADER pfl_header = &_fl_header;

	ReleaseSection();
	
	pfl_header->pSection = (FL_SECTION_INF *)malloc(pfl_header->nSection*sizeof(FL_SECTION_INF));
	if(pfl_header->pSection == NULL)
	{
		printf("Malloc fail!\n");
		return 0;
	}
	
	fread(pfl_header->pSection, pfl_header->nSection*sizeof(FL_SECTION_INF), 1, g_prf);	
	return 1;
}

void ReleaseSection()
{
	if(_fl_header.pSection != NULL)
	{
		free(_fl_header.pSection);
		_fl_header.pSection = NULL;
	}
}

// ��ȡ�ַ������ؿ��
DWORD ReadCharDistX_U(WORD wCode)
{	
	DWORD offset ;
	int   i;
	g_dwCharInfo = 0;
	
	for(i = 0;i<_fl_header.nSection;i++)
	{
		if(wCode >= _fl_header.pSection[i].First && wCode <= _fl_header.pSection[i].Last)
			break;		
	}
	if(i >= _fl_header.nSection)	
		return 0;	
	
	offset = _fl_header.pSection[i].OffAddr+ FONT_INDEX_TAB_SIZE*(wCode - _fl_header.pSection[i].First );	
	fseek(g_prf, offset, SEEK_SET);
	fread(&g_dwCharInfo, sizeof(DWORD), 1, g_prf);	

	return GET_FONT_WIDTH(g_dwCharInfo);
}





