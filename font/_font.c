
#include "unicode.h"
#include "mbcs.h"


void print_font_info(Font_t *pfont)
{
	int i;
	printf("*******************************font: ********************\r\n");
	printf("* font encode type: %s \r\n",pfont->header->magic[0] == 'M' ? "MBCS" : "UNICODE");
	if(pfont->header->magic[0]=='U' && pfont->header->magic[0]=='F')
	{
		printf("********* font section info, total section num : %d \r\n",pfont->header->nSection);
		for(i=0;i<pfont->header->nSection;i++)
		{
			printf("*\t section %d: start font:%04x, end font:%04x, offset:%08x \r\n",
				i,pfont->pSection[i]->First,pfont->pSection[i]->Last,pfont->pSection[i]->OffAddr);
		}
	}
	printf("*****************************************************************\r\n");
}

int init_font(Font_t *pfont)
{

	if((pfont->header->magic[0] != 'U' && pfont->header->magic[0] != 'M')
		|| (pfont->header->magic[1] != 'F' && pfont->header->magic[1] != 'P') || pfont->header->magic[2] != 'L')
	{
		printf("Cann't support file format   !\n");
		return -1;
	}

	if('U' == pfont->header->magic[0]) 	//unicode ±àÂë
	{
		pfont->pSection=(FL_SECTION_INF **)((LiteU8 *)pfont->header+sizeof(FL_HEADER));
	}
	else{
		pfont->pSection = (FL_SECTION_INF **)NULL;
	}
	return 0;	
}

int destroy_font(Font_t *pfont)
{
	return 0;
}




