// getwcode.cpp : Defines the entry point for the console application.
//

//#include <fcntl.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <io.h>
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <string.h>

// CJK Unified Ideographs Extension A  3400-4DB5 
// CJK Unified Ideographs  4E00-9FFF 
// CJK Compatibility Ideographs  F900-FAFF 
// Hangul Jamo  1100-11FF 
// Hangul Syllables  AC00-D7A3 

typedef struct
{
	unsigned short start;
	unsigned short end;
}ch_range_t;

ch_range_t chi_range[] = 
{
	{0x3400,0x4DB5},
	{0x4E00,0x9FFF},
	{0xF900,0xFAFF},
};

ch_range_t han_range[] = 
{
	{0xAC00,0xD7A3},
};


typedef unsigned short _u16;

int getcode(const char* inputfilename, const char* outfilename, const char* outfilename2,
			const char* var_name)
{
	FILE*	fd1 = NULL;
	FILE*	fd2 = NULL;
	FILE*	fd3 = NULL;

	int i,filesize=0;
	unsigned char* str = NULL;
	unsigned char hdr_unicode[] = {0xFF,0xFE};
	unsigned char crcn[] = {0x0D,0x00,0x0A,0x00};
	unsigned short wcode_list[1024];
	int wcode_cnt=0;
	int len=0;
	int mcnt=0;
	unsigned short wch=0;
	int exist=0;
	long pos;
	char tmp[10*1024];

	memset(wcode_list,0,sizeof(wcode_list));

	fd1 = fopen(inputfilename,"rb");
	if (fd1 == NULL)
	{
		fprintf(stderr,"%s failed\n",inputfilename);
		fclose(fd1);
		return -1;
	}

	fd2 = fopen(outfilename,"wb+");
	if (fd2 == NULL)
	{
		fprintf(stderr,"%s failed\n",outfilename);
		fclose(fd1);
		fclose(fd2);
		return -1;
	}

	fd3 = fopen(outfilename2,"wb+");
	if (fd3 == NULL)
	{
		fprintf(stderr,"%s failed\n",outfilename2);
		fclose(fd1);
		fclose(fd2);
		fclose(fd3);
		return -1;
	}

	filesize = fseek(fd1,0,SEEK_END);
	printf("original file size: %d \n",filesize);
	filesize = ftell(fd1);
	printf("original file size: %d \n",filesize);
	fseek(fd1,0,SEEK_SET);

	str = (unsigned char*)malloc(filesize);

	if(!str)
	{
		fclose(fd1);
		fclose(fd2);
		printf("malloc failed\r\n");
		return 0;
	}

	fread(str,1,filesize,fd1);

	mcnt = 1;

	// 遍历整个文件
	while(1)
	{
		if((mcnt+1) >= filesize)
			break;

		wch = 0;
		exist = 0;
		if(str[mcnt] == 0)
			break;

		if((str[mcnt] & 0x80) == 0)
		{
			wch = (_u16)(str[mcnt]);
			mcnt++;
		}
		else if((str[mcnt] & ~0x1f) == 0xc0)
		{
			wch = 0x1f & (_u16)str[mcnt];
			mcnt++;
			if(str[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(_u16)str[mcnt]);
			mcnt++;
		}
		else if((str[mcnt] & ~0x0f) == 0xe0)
		{
			wch = 0x0f & (_u16)str[mcnt];
			mcnt++;
			if(str[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(_u16)str[mcnt]);
			mcnt++;
			if(str[mcnt] == 0)
				break;
			wch = (wch<<6) | (0x3f&(_u16)str[mcnt]);
			mcnt++;
		}
		else
		{
			mcnt++;
		}

		if(wch > 0x007E)
		{
			for(i=0; i < wcode_cnt; i++)
			{
				if(wcode_list[i] == wch)
				{
					exist = 1;
					break;
				}
			}

			if(exist ==0 && wcode_cnt < 1024)
			{
				wcode_list[wcode_cnt] = wch;
				wcode_cnt++;
			}
		}
	}

	printf("total wcode = %d\r\n",wcode_cnt);

	fwrite(hdr_unicode,1,2,fd2);

	int w=0;
	for(w=0; w < wcode_cnt; w++)
	{
		fwrite(&wcode_list[w],1,2,fd2);
		if((w%8) == 7)
		{
			fwrite(crcn,1,4,fd2);
		}
	}

	if(var_name)
	{
		sprintf(tmp,"const unsigned short %s[] = \r\n",var_name);
		fwrite(tmp,1,strlen(tmp),fd3);
	}

	sprintf(tmp,"{\r\n");
	fwrite(tmp,1,strlen(tmp),fd3);

	for(i=0; i < wcode_cnt; i++)
	{
		sprintf(tmp,"0x%02x,",wcode_list[i]);
		fwrite(tmp,1,strlen(tmp),fd3);
		if((i%8) == 7)
		{
			sprintf(tmp,"\r\n");
			fwrite(tmp,1,strlen(tmp),fd3);
		}
	}

	sprintf(tmp,"\r\n};\r\n");
	fwrite(tmp,1,strlen(tmp),fd3);

	fclose(fd1);
	fclose(fd2);
	fclose(fd3);

	free(str);

  	return 0;
}

int main(int argc, char* argv[])
{

	if(!(argc == 5))
	{
		printf("usage:");
		printf("getwcode [input filename] [output filename] [output filename (c style)] [var name]\r\n");
		return 0;
	}

	getcode(argv[1],argv[2],argv[3],argv[4]);

	return 0;
}

