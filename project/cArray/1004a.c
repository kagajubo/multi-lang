#include "1004a.h"
#include "font.h"




FILE *g_pCodefile;
FILE *g_pCArrayfile;
unsigned short g_codeArray[MAX_CODES];
char gsz_lang[32];

extern FL_HEADER _fl_header;
extern DWORD g_dwCharInfo;

static unsigned short convHexStrToU16(char *str)
{
	unsigned short num=0, i;

	for (i=0;; i++)
	{
		if (str[i]==0)
			break;
		if (str[i]>='0' && str[i]<='9')
			num = (num<<4) + str[i] - '0';
		else if (str[i]>='A' && str[i]<='F')
			num = (num<<4) + str[i] - 'A' + 10;
		else if (str[i]>='a' && str[i]<='f')
			num = (num<<4) + str[i] - 'a' + 10;
	}
	return num;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////// font_unicode_set_xxx.txt //////////////////////////////////////////////////////////////
/************************************************** 
file content as below:
ABCDEFGH
IJKLMNOP
QRST
****************************************************/
static int openCodeFile_txt(char *pCodeFile)
{
	if((g_pCodefile = fopen(pCodeFile, "rb")) == NULL)
	{
		FW_ERR("Cann't open : %s\n", pCodeFile);
		return -1;
	}
	
	return 0;
}
static int CloseCodeFile_txt()
{
	if(g_pCodefile)
		fclose(g_pCodefile);
	return 0;
}
static int ParseCodeFile_txt(char *pCodeFile)
{
	int i=0;
	openCodeFile_txt(pCodeFile);
	if(g_pCodefile==NULL)
		return -1;
	unsigned short flag=0;
	unsigned short tmp=0;
	fread(&flag,2,1,g_pCodefile);
	if(flag!=0xfeff)
	{
		FW_ERR("not txt file \r\n");
		return -1;
	}
	while(fread(&tmp,2, 1, g_pCodefile)==1)
	{
		if(tmp!= 0x0d && tmp!=0x0a)			
			g_codeArray[i++]=tmp;
		if(i>=MAX_CODES)
		{
			FW_ERR("ERROR: out of max codes \r\n");
			break;
		}
	}
	CloseCodeFile_txt();
	return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////// font_table_xxx.h //////////////////////////////////////////////////////////////
/************************************************** 
const unsigned short font_unicode_table_thai[] = 
{
0xe08,0xe31,0xe14,0xe15,0xe49,0xe07,0xe02,0xe36,
0xe19,0xe01,0xe25,0xe2d,0xe23,0xe30,0xe40,0xe1a,
0xe1f,0xe24,0xe10,0xe0f,0xe18,
};
****************************************************/
static int openCodeFile_h(char *pCodeFile)
{
	if((g_pCodefile = fopen(pCodeFile, "rb")) == NULL)
	{
		FW_ERR("Cann't open : %s\n", pCodeFile);
		return -1;
	}
	
	return 0;
}
static int CloseCodeFile_h()
{
	if(g_pCodefile)
		fclose(g_pCodefile);
	return 0;
}
static int ParseCodeFile_h(char *pCodeFile)
{
	int i=0,j=0,k=0;
	char temp[1024*100]={0,};
	unsigned short flag=0;
	char tmp[10]={0,};
	int read_byte=0;
	
	openCodeFile_h(pCodeFile);
	if(g_pCodefile==NULL)
		return -1;

	read_byte=fread(&flag,sizeof(temp),1,g_pCodefile);
	FM_DBG("read byte : %d \r\n",read_byte);
	while(i<(read_byte-1))
	{
		if(temp[i]=='0' && temp[i+1]=='x')
		{
			j=0;
			i+=2;	//jump "0x"
			do{
				tmp[j++]=temp[i++];
			}while(temp[i]!=',');
			g_codeArray[k++]=convHexStrToU16(temp);
		}
		i++;	
	}
	FM_DBG("get total code:%d \r\n",k);
	#ifdef DEBUG_1004A
	for(i=0;i<k;i++)
	{
		if(k%8==0) printf("\r\n");
		printf("%04x,",g_codeArray[i]);
	}
	printf("\r\n");
	#endif
	CloseCodeFile_h();
	return k;
}

int MakeCodeFile_h(unsigned short s,unsigned p)
{
#define MAKE_WIDTH_TABLE_TO_CODE_FILE
	FILE * pf;
	int nBytesPerLine;
	char *filename[128]={0,};
	char tmp[1024*10]={0,};
	unsigned short cur=0;
	int count;
	GET_FONT_TABLE_FILE_NAME(filename,gsz_lang);
	pf=fopen(filename,"w+");
	if(pf==NULL)
	{
		FM_ERR("create file:%s error\r\n",filename);
		return -1;
	}
	sprintf(tmp,"#ifndef __font_unicode_table_%s_h__\r\ndefine __font_unicode_table_%s_h__\r\n\r\n",
		gsz_lang,gsz_lang);
	fwrite(tmp,strlen(tmp),1,pf);
	sprintf(tmp,"const unsigned short font_unicode_table_%s[] = \r\n{\r\n",gsz_lang);
	fwrite(tmp,strlen(tmp),1,pf);
	for(cur=s;cur<=p;cur++)
	{
		if(ReadCharDistX(cur)==0)	//to get g_dwCharInfo
		{
			FM_DBG("char:0x%04x(%d) not exist\r\n",cur,cur);
			continue;
		}
		count++;
		if(count%8==0) 
			sprintf(tmp,"0x%04x,\r\n",cur);
		else
			sprintf(tmp,"0x%04x,",cur);
		fwrite(tmp,strlen(tmp),1,pf);
	}
	strcpy(tmp,"\r\n};\r\n#endif");
	fwrite(tmp,strlen(tmp),1,pf);

#ifdef 	MAKE_WIDTH_TABLE_TO_CODE_FILE
	int nXSize=0;
	int n_word=0;
	count=0;
	sprintf(tmp,"const unsigned char font_width_table_%s[] = \r\n{\r\n",gsz_lang);
	fwrite(tmp,strlen(tmp),1,pf);
	for(cur=s;cur<=p;cur++)
	{
		if((nXSize=ReadCharDistX(cur))==0)	//to get g_dwCharInfo
		{
			FM_DBG("char:0x%04x(%d) not exist\r\n",cur,cur);
			continue;
		}
		n_word=(nXSize+3)/4*4;
		count++;
		if(count%8==0) 
			sprintf(tmp,"%d/*%04x*/, \r\n",n_word,cur);
		else
			sprintf(tmp,"%d/*%04x*/, ",n_word,cur);
		fwrite(tmp,strlen(tmp),1,pf);
	}
	sprintf(tmp,"\r\n};//total num:%d\r\n",count);
	fwrite(tmp,strlen(tmp),1,pf);
#endif
	if(pf)	fclose(pf);
	return 0;
}

int MakeFontWidthFile_h(unsigned short s,unsigned p)
{
	FILE * pf;
	int nBytesPerLine;
	char *filename[128]={0,};
	char tmp[1024*10]={0,};
	unsigned short cur=0;
	int nXSize=0,n_word=0;
	int count=0;
	GET_FONT_WIDTH_FILE_NAME(filename,gsz_lang);
	pf=fopen(filename,"w+");
	if(pf==NULL)
	{
		FM_ERR("create file:%s error\r\n",filename);
		return -1;
	}
	
	sprintf(tmp,"const unsigned char font_width_table_%s[] = \r\n{\r\n",gsz_lang);
	fwrite(tmp,strlen(tmp),1,pf);
	for(cur=s;cur<=p;cur++)
	{
		if((nXSize=ReadCharDistX(cur))==0)	//to get g_dwCharInfo
		{
			FM_DBG("char:0x%04x(%d) not exist\r\n",cur,cur);
			continue;
		}
		n_word=(nXSize+3)/4*4;
		count++;
		if(count%8==0) 
			sprintf(tmp,"%d/*%04x*/, \r\n",n_word,cur);
		else
			sprintf(tmp,"%d/*%04x*/, ",n_word,cur);
		fwrite(tmp,strlen(tmp),1,pf);
	}
	sprintf(tmp,"\r\n};//total num:%d\r\n",count);
	fwrite(tmp,strlen(tmp),1,pf);
	if(pf)	fclose(pf);
	return 0;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////font_%dx24_%s.h /////////////////////////////////////////////////////////////
/**************************************************************
* file content format, just as below:
unsigned long font_12x24_eng_palette[] = 
{
	0x00000000,
	0x80108010,
	0x80eb80eb,
	0x10d292d2,
	0x807d807d,
	0x5a51ef51,
	0x35902290,
	0xb4386238,
	0x805f805f,
	0x80b480b4,
	0x80cc80cc,
	0x4e73cd73,
	0x5fa6a8a6,
	0x5a505050,
	0x10d292d2,
	0xb4386238,
};

const unsigned short font_12x24_eng_0[] = {
0x1111,0x1111,0x1111,		//total 24 lines	
0x1111,0x1111,0x1111,
...
...
};
const unsigned short font_12x24_eng_120[] = {
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x2221,0x1111,0x2221,
0x2222,0x1112,0x2221,
0x1222,0x1112,0x2211,
0x1122,0x2122,0x2111,
0x1112,0x2222,0x1111,
0x1111,0x2221,0x1111,
0x1111,0x2221,0x1111,
0x1111,0x2221,0x1111,
0x1112,0x2222,0x1111,
0x1122,0x2122,0x2111,
0x1222,0x1112,0x2211,
0x2222,0x1112,0x2221,
0x2221,0x1111,0x2221,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
0x1111,0x1111,0x1111,
};
...
...
const unsigned short* font_12x24_eng[] = {
font_12x24_eng_0,
font_12x24_eng_1,
...
...
};
***************************************************************/
static int openCArrayFile()
{
	char name[128]={0,};
	GET_FONT_FILE_NAME(name,gsz_lang);
	if((g_pCArrayfile= fopen(name, "w+")) == NULL)
	{
		FW_ERR("Cann't open : %s\n", name);
		return -1;
	}
	
	return 0;
}
static int CloseCArrayFile()
{
	if(g_pCArrayfile)
		fclose(g_pCArrayfile);
	return 0;
}


const char * sz_palette="\r\n"
"unsigned long font_XXx24_%s_palette[] = \r\n"
"{\r\n"
"	0x00000000,\r\n"
"	0x80108010,\r\n"
"	0x80eb80eb,\r\n"
"	0x10d292d2,\r\n"
"	0x807d807d,\r\n"
"	0x5a51ef51,\r\n"
"	0x35902290,\r\n"
"	0xb4386238,\r\n"
"	0x805f805f,\r\n"
"	0x80b480b4,\r\n"
"	0x80cc80cc,\r\n"
"	0x4e73cd73,\r\n"
"	0x5fa6a8a6,\r\n"
"	0x5a505050,\r\n"
"	0x10d292d2,\r\n"
"	0xb4386238,\r\n"
"};\r\n";

const unsigned short oct2quad[16]=	//1 for black ,2  for white
{
0x1111,	//0000
0x1112,	//0001
0x1121,	//
0x1122,
0x1211,
0x1212,
0x1221,
0x1222,
0x2111,
0x2112,
0x2121,
0x2122,
0x2211,
0x2212,
0x2221,
0x2222
};
static int _1004A_makeCArray(unsigned short s,unsigned short p)
{
	int i=0,j=0,k=0;
	int nBytesPerLine;
	int nXSize;
	unsigned short cur=0;	//current unicode code
	int count=0;	//how many codes has written?
	int n_word=0;	//1004a使用16色的调色板，硬件接口一写就写4个点，即一个word(2个byte)
					//用于取nxsize的邻近的4 的整数倍
	char tmp[1024*5]={0,};
	//char *ptmp=tmp;
	char buf[1024]={0,};
	//open file
	if(openCArrayFile())
		return -1;
	//------------write palette
	sprintf(tmp,sz_palette,gsz_lang);
	fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
	//-------------fonts array
	for(cur=s;cur<=p;cur++)
	{
		//get char real width
		nXSize=ReadCharDistX(cur);	//to get g_dwCharInfo
		n_word=(nXSize+3)/4;
		//read char dot array
		if(ReadCharDotArray(cur, buf, &nBytesPerLine)==0)
		{
			FM_ERR("ReadCharDotArray error or char not exist\r\n");
			//return -1;
			continue;
		}
		//----------------array start
		sprintf(tmp,"const unsigned short font_XXx24_%s_%d[] = {\r\n//font:,CODE=0x%04x(%d),CX=%d,CY=%d\r\n",gsz_lang,count,cur,cur,n_word*4,_fl_header.YSize);
		fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
		//----------------array content
		FM_DBG("cur=%04x,nXSize=%d,n_word=%d(%d), nBytesPerLine=%d\r\n",cur,nXSize,n_word,n_word*4,nBytesPerLine);
		for(i = 0; i < _fl_header.YSize; i++)	
		{
			int r_word=0;
			for(j = 0; j < nBytesPerLine; j++)
			{
				//printf("%x, ", buf[i * nBytesPerLine + j]);
				char temp=buf[i*nBytesPerLine+j];
				sprintf(tmp,"0x%04x,",oct2quad[(temp >>4) & 0x0F]);
				fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
				k++;
				if(nXSize>4)
				{
					if((k%(nXSize/4))==0) 
					{
						sprintf(tmp,"\r\n");
						fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
					}
				}
				else{
					sprintf(tmp,"\r\n");
					fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
				}
				if((++r_word)==n_word) break;
				/////////////////////////////////////////////
				sprintf(tmp,"0x%04x,",oct2quad[temp  & 0x0F]);
				fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
				k++;
				if(nXSize>=4)
				{
					if((k%(nXSize/4))==0) 
					{
						sprintf(tmp,"\r\n");
						fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
					}
				}
				else{
					sprintf(tmp,"\r\n");
					fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
				}
				if((++r_word)==n_word) break;
			}
			//printf("\n");
		}
		//------------------array end
		sprintf(tmp,"\r\n};\r\n\r\n",gsz_lang,count);
		fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
		
		count++;
		k=0;
	}
	//-------------array for all fonts
	//-------------start
	sprintf(tmp,"const unsigned short const*const font_XXx24_%s[] = {\r\n",gsz_lang);
	fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
	//-----------content
	for(i=0;i<count;i++)
	{
		sprintf(tmp,"font_XXx24_%s_%d,\r\n",gsz_lang,i);
		fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
	}
	//------------end
	sprintf(tmp,"\r\n};\r\n\r\n");
	fwrite(tmp,strlen(tmp),1,g_pCArrayfile);
	//+++++++++++++end of array for all fonts
	CloseCArrayFile();
	return 0;
}


int  _1004A_CArray(unsigned short startCode,unsigned short endCode,char *pCodeFile,char *lang)
{
	int codeNum;
	char filename[128];
	unsigned short cur;
	int count=0;
	gsz_lang[0]=0;
	strcpy(gsz_lang,lang);
	GET_FONT_TABLE_FILE_NAME(filename,lang);
	if(pCodeFile)
	{
		if((codeNum=ParseCodeFile_h(filename))==-1)
			return -1;
		
	}
	else{
		//make code file
		MakeCodeFile_h(startCode,endCode);
		//FONT WIDTH FILE
		MakeFontWidthFile_h(startCode,endCode);
		//make font array file
		_1004A_makeCArray(startCode,endCode);
	}
}
