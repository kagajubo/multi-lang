#include "../lite.h"
#include "lite_font.h"


#include "./wordstock/wordstock_eng.h"
#ifdef LITE_S_CHINESE
#include "./wordstock/wordstock_sch.h"
#endif
#ifdef LITE_RUSSIAN
#include "./wordstock/wordstock_rus.h"
#endif
#ifdef LITE_KOREAN
#include "./wordstock/wordstock_kor.h"
#endif

const LiteU8* word_stock[]={
	wordstock_eng,
#ifdef LITE_S_CHINESE
	wordstock_sch,
#endif
#ifdef LITE_RUSSIAN
	wordstock_rus,
#endif
#ifdef LITE_KOREAN
	wordstock_kor,
#endif

};

static LiteFont _eng_font;
static LiteFont _oth_font;


static int _utf8_to_unicode(const LiteU8 *utf8,LiteU16 *unicode,LiteU16 buf_size)
{
	LiteU16 wch=0;
	LiteU16 mcnt=0;
	LiteU16 len=0;
	if(unicode==NULL){
		LITE_ERR("input parameter error.");
		return -1;
	}
	while(1)
	{
		wch = 0;
		
		if(utf8[mcnt] == 0)	//end of string
			break;
	
		if((utf8[mcnt] & 0x80) == 0) // ASCII characters.
		{
			wch = (unsigned short)(utf8[mcnt]);
			mcnt++;	
		}
		else if((utf8[mcnt] & ~0x1f) == 0xc0)
		{
			wch = 0x1f & (unsigned short)utf8[mcnt];
			mcnt++;
			if(utf8[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(unsigned short)utf8[mcnt]);
			mcnt++;
		}
		else if((utf8[mcnt] & ~0x0f) == 0xe0)
		{
			wch = 0x0f & (unsigned short)utf8[mcnt];
			mcnt++;
			if(utf8[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(unsigned short)utf8[mcnt]);
			mcnt++;
			if(utf8[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(unsigned short)utf8[mcnt]);
			mcnt++;
		}		
		else
		{
			mcnt++;
		}
		
		if(len>buf_size){
			LITE_ERR("buffer overflow.");
			return -1;
		}
		unicode[len++]=wch;
	}
	unicode[len]=0;
	//LITE_DEBUG("total font number: %d ",len);
	
#ifdef DEBUG_LITE_FONT
	int i;
	for(i=0;i< len;i++)
		printf("%#06x",unicode[i]);
#endif

	return len;
}


int lite_font_init(LiteLangType lang)
{
	LiteFont *cur_font;
	if(lang== LITE_LANG_ENG){
		cur_font = &_eng_font;
		_eng_font.lang=lang;
		_eng_font.header=(FL_HEADER *)word_stock[0];
	}
	else
	{
		cur_font = &_oth_font;
		_oth_font.lang=lang;
		_oth_font.header=(FL_HEADER *)word_stock[lang];
	}
	//printf("start: %p end : %p \n",arialuni24_CP1252,arialuni24_CP1252+sizeof(arialuni24_CP1252));
	if(init_font(cur_font)==0)
	{
		print_font_info(cur_font);
		return 0;
	}
	else
		return -1;
		
}

int lite_font_destroy()
{
	return 0;
}

static int lite_font_info(LiteU16 unicode,LiteCharInfo *char_info)
{
	int nWidth = 0,size=0;
	LiteU16 bytesPerLine = 0;
	LiteU8 *pbuf=NULL;
	LiteFont *cur_font = &_eng_font;

	if(unicode > 0x7f)
		cur_font = &_oth_font;

	
	if('U' == cur_font->header->magic[0])     //unicode ±àÂë
	{
		if(USE_PARTIAL_CODEPAGE_FLAG == cur_font->header->magic[1])
			nWidth = ReadCharDistX_UP(cur_font,unicode);
		else
			nWidth = ReadCharDistX_U(cur_font,unicode);
	}
	else
		nWidth = ReadCharDistX_M(cur_font,unicode);

#ifdef DEBUG_LITE_FONT
	printf("wCode = 0x%x width=%d\n",  unicode,nWidth);
#endif
	if(nWidth ==0)
		return -1;
#ifdef _BIG_ENDIAN
#ifdef CONVERT_TO_BIG_ENDIAN
	cur_font->dwCharInfo.padding=font_swab32(cur_font->dwCharInfo.padding);
#endif
	if(USE_PARTIAL_CODEPAGE_FLAG == cur_font->header->magic[1]){
		pbuf=(LiteU8 *)(((LiteU8 *)cur_font->header) +cur_font->dwCharInfoEx.be_format.OffAddr);
		bytesPerLine=(LiteU16)(cur_font->dwCharInfoEx.be_format.Width+7)/PIXELS_PER_BYTE;
	}
	else{
		pbuf=(LiteU8 *)(((LiteU8 *)cur_font->header) +cur_font->dwCharInfo.be_format.OffAddr);
		bytesPerLine=(LiteU16)(cur_font->dwCharInfo.be_format.Width+7)/PIXELS_PER_BYTE;
	}
#else // _LITTLE_ENDIAN
#ifdef CONVERT_TO_LITTLE_ENDIAN
	cur_font->dwCharInfo.padding = font_swab32(cur_font->dwCharInfo.padding);
#endif
	if(USE_PARTIAL_CODEPAGE_FLAG == cur_font->header->magic[1]){
		pbuf=(LiteU8 *)(((LiteU8 *)cur_font->header) +cur_font->dwCharInfoEx.le_format.OffAddr);
		bytesPerLine=(LiteU16)(cur_font->dwCharInfoEx.le_format.Width+7)/PIXELS_PER_BYTE;
	}
	else{
		pbuf=(LiteU8 *)(((LiteU8 *)cur_font->header) +cur_font->dwCharInfo.le_format.OffAddr);
		bytesPerLine=(LiteU16)(cur_font->dwCharInfo.le_format.Width+7)/PIXELS_PER_BYTE;
	}
#endif //#ifdef _BIG_ENDIAN

	size=cur_font->header->YSize;

#ifdef DEBUG_LITE_FONT
#ifdef _LITTLE_ENDIAN
	if(USE_PARTIAL_CODEPAGE_FLAG == cur_font->header->magic[1])
		printf("le:wCode = 0x%x width=%d pdata=%p size=%d offset=%#10x, width:%d\n",  
			unicode,nWidth,pbuf,size,cur_font->dwCharInfoEx.le_format.OffAddr,cur_font->dwCharInfoEx.le_format.Width);
	else
		printf("le:wCode = 0x%x width=%d pdata=%p size=%d offset=%#10x, width:%d\n",  
			unicode,nWidth,pbuf,size,cur_font->dwCharInfo.le_format.OffAddr,cur_font->dwCharInfo.le_format.Width);
#else
	if(USE_PARTIAL_CODEPAGE_FLAG == cur_font->header->magic[1])
		printf("be:wCode = 0x%x width=%d pdata=%p size=%d offset=%#10x, width:%d\n",  
			unicode,nWidth,pbuf,size,cur_font->dwCharInfoEx.be_format.OffAddr,cur_font->dwCharInfoEx.be_format.Width);
	else
		printf("be:wCode = 0x%x width=%d pdata=%p size=%d offset=%#10x, width:%d\n",  
			unicode,nWidth,pbuf,size,cur_font->dwCharInfo.be_format.OffAddr,cur_font->dwCharInfo.be_format.Width);
#endif
	DisplayChar(size, bytesPerLine, pbuf);
#endif
	char_info->width= nWidth;
	char_info->size=size;
	char_info->pdata=pbuf;
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////
/////

#if 1
int lite_font_bitblit(LiteCharInfo *char_info, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteColor color,LiteU8 size)
{
	int i = 0, ii = 0;
	int width=char_info->width;
	LiteColor *dst_drawing;
	LiteU8 *src_pixel;
	LiteU8 pixel;

	if(!dst)
		dst=lite_layer(0);
	
	src_pixel=char_info->pdata;
	for(i = 0; i < char_info->size; ++i){
		dst_drawing=(LiteColor *)((y+i)*dst->w+x+dst->pixels);
		for(ii = 0; ii < width; ++ii){
			if(ii%8==0)
				pixel=*src_pixel++;
			if(pixel & 0x80)
				*dst_drawing++=color;
			else
				dst_drawing++;
			
			pixel <<=1;
		}
	}
	return width;
}

#else
static int lite_font_get_pixel(LiteCharInfo *char_info, int x, int y)
{
	LiteU8 linebytes=(char_info->width+8-1)/8;
	LiteU8 byte_off=y*linebytes+x/8;
	LiteU8 bit_off=x%8;
	LiteU8 pixel=*(char_info->pdata+byte_off);
	if ( pixel & (0x80 >> bit_off)){
		return 1;
	}
	else
		return 0;
}

int lite_font_bitblit(LiteCharInfo *char_info, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteColor color,LiteU8 size)
{
	int i = 0, ii = 0,n=0,m=0;
	int width=char_info->width;
	LiteColor *dst_drawing;
	LiteU8 *src_pixel;
	LiteU8 pixel;
	LiteU8 scale=0;
	LiteU8 in;
	
	if(!dst)
		dst=lite_layer(0);
	
	if(size==0)
		size=char_info->size;
	
	if(size == char_info->size){
		scale=0;
		
		for(i = 0; i < char_info->size; ++i){
			for(ii = 0; ii < width; ++ii){
				if(lite_font_get_pixel(char_info,ii,i)){
					lite_put_pixel(dst,x+ii,y+i,color);
				}
			}
		}
		return width;
	}
	else if(size%char_info->size==0){
		scale=size/char_info->size;

		for(i = 0; i < char_info->size; ++i){
			for(ii = 0; ii < width; ++ii){
				if(lite_font_get_pixel(char_info,ii,i)){
					
					for (m=0;m< scale;m++){
						for (n=0;n< scale;n++){
							lite_put_pixel(dst,x+ii*scale+n,y+i*scale+m,color);
						}
					}
				}
			}
		}
		return width*scale;
	}
	else if(size < (char_info->size*2)){
		scale=1;
		in=char_info->size/(size-char_info->size);
		for(i = 0; i < char_info->size; ++i){
			for(ii = 0; ii < width; ++ii){
				if(lite_font_get_pixel(char_info,ii,i)){
					if((ii%in==0) || (i%in==0)){
					}
					else
						lite_put_pixel(dst,x+ii,y+i,color);
				}
			}
		}
		
	}

}
#endif

int lite_default_font_size()
{
	return ORIGINAL_FONT_SIZE;
}
int lite_char(LiteU16 code, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteColor color)
{
	LiteCharInfo achar;
	if(lite_font_info(code,&achar)< 0)
		return -1;

	lite_font_bitblit(&achar,dst,x,y,color,0);
	return 0;
}

#if (DEFAULT_FONT_ENCODE == FONT_ENCODE_ANSI)
int lite_string_width(LiteChr *string)
{
	LiteCharInfo achar;
	LiteU16 code;
	int num=0,len=0,i=0;
	if(string){
		for(i=0;string[i];i++)
		{
			if(string[i] & 0x80) // non ascii
			{
				if(string[i+1] & 0x80){
					code=(((LiteU16)string[i])<<8) | ((LiteU16)string[i+1]);
					i++;
				}
				else{ // not quite clearly (128-255)
					code= string[i];
				}
			}
			else{// ascii
				code=string[i];
			}
			if(lite_font_info(code,&achar)<0)
				return -1;
			num++;
			len+=achar.width;
		}
		//printf("total wchar: %d ,total length: %d\n",num,len);
		return len;
	}
	else
		return -1;
}

int lite_string(LiteChr *string, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteU16 w,LiteU16 h,LiteColor color)
{
	LiteCharInfo achar;
	LiteU16 code;
	int len=0,i=0;
	LiteU16 sy=y;
	LiteU16 sx=x;
	if(string){
		for(i=0;string[i];i++)
		{
			if(string[i] & 0x80) // non ascii
			{
				if(string[i+1] & 0x80){
					code=(((LiteU16)string[i])<<8) | ((LiteU16)string[i+1]);
					i++;
				}
				else{ // not quite clearly (128-255)
					code= string[i];
				}
			}
			else{// ascii
				code=string[i];
			}
			len++;
			if(lite_font_info(code,&achar)<0)
				return -1;
			if( w > 0 && ((x + achar.width) > (w+sx))){
				y+=achar.size;
				x=sx;
			}
			if( (h > 0) && ((y+achar.size) > (sy+h))){
				LITE_ERR("exceed input area!!!");
				return -1;
			}
			x+=lite_font_bitblit(&achar,dst,x,y,color,achar.size);
		}
		//printf("total wchar: %d \n",len);
		return 0;
	}
	else
		return -1;
}

#elif (DEFAULT_FONT_ENCODE == FONT_ENCODE_UNICODE)
#elif (DEFAULT_FONT_ENCODE == FONT_ENCODE_UTF8)
int lite_string_width(LiteChr *string)
{
#define MAX_CODES_IN_ASTRING	(128)
	LiteCharInfo achar;
	LiteU16 unicodes[MAX_CODES_IN_ASTRING];
	int num=0,len=0,i=0;
	if(string){
		num=_utf8_to_unicode((LiteU8 *)string,unicodes,MAX_CODES_IN_ASTRING);
		for(;unicodes[i];i++)
		{
			if(lite_font_info(unicodes[i],&achar)<0)
				return len;//-1;
			len+=achar.width;
		}
		//printf("in lite_string_width:::total wchar: %d ,total length: %d\n",num,len);
		return len;
	}
	else
		return -1;
#undef MAX_CODES_IN_ASTRING
}

int lite_string(LiteChr *string, LiteSurface* dst, LiteU16 x,LiteU16 y,LiteU16 w,LiteU16 h,LiteColor color)
{
#define MAX_CODES_IN_ASTRING	(128)
	LiteCharInfo achar;
	LiteU16 unicodes[MAX_CODES_IN_ASTRING];
	int len=0,i=0;
	LiteU16 sy=y;
	LiteU16 sx=x;
	if(string){
		len=_utf8_to_unicode((LiteU8 *)string,unicodes,MAX_CODES_IN_ASTRING);
		for(;unicodes[i];i++)
		{
			if(unicodes[i]!=0x0a){
				if(lite_font_info(unicodes[i],&achar)<0)
					return -1;
			}
			
			if( (w > 0 && ((x + achar.width) > (w+sx))) || (unicodes[i]==0x0a)){
				y+=achar.size;
				x=sx;
			}
			if( (h > 0) && ((y+achar.size)> (sy+h))){
				LITE_ERR("exceed input area!!!");
				return -1;
			}
			x+=lite_font_bitblit(&achar,dst,x,y,color,achar.size);
		}
		//printf("total wchar: %d \n",len);
		return 0;
	}
	else
		return -1;
#undef MAX_CODES_IN_ASTRING
}
#else
#endif


