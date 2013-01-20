#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../_font.h"

#define TRUE 	(1)
#define FALSE 	(0)

#define MAX_BUF_SIZE	(1024*1024*5)
#define READ_SIZE	(32*1024)
FILE *in_file;
FILE *out_file;
FILE *code_file;


//unsigned char *buf_in;
//unsigned char *buf_out;
unsigned char buf_in[MAX_BUF_SIZE];
unsigned char buf_out[MAX_BUF_SIZE];

unsigned char *pbuf_in;
unsigned char *pbuf_out;

char sz_input[128];
char sz_output[128];

unsigned long infile_size;
unsigned long outfile_size;



int to_big_endian()
{
	int i;
	unsigned short j;
	int ret;
	FL_HEADER *pheader;

	//copy total input to output
	memcpy(buf_out,buf_in,infile_size);
	outfile_size=infile_size;

	// do with header
	pheader = buf_in;
	pbuf_in=buf_in;
	if(!((pheader->magic[0] == 'U' || pheader->magic[0] == 'M') &&
		(pheader->magic[1] == 'F' || pheader->magic[1] == 'P') && (pheader->magic[2] == 'L'))){
		printf("not valid file\n");
		return -1;
	}
	if(pheader->Size != infile_size){
		printf("read size not equal to the recorded size.\n");
		return -1;
	}
	else{
		printf("valid file, code by %s version: %02x \n",
			pheader->magic[0]== 'U' ? "unicode" : "MBCS",pheader->magic[3]);
	}
	//
	//pheader->Size = font_swab32(pheader->Size);
	pheader->wCpFlag=font_swab16(pheader->wCpFlag);
	memcpy(pbuf_out,(char *)pheader,sizeof(FL_HEADER));
	pbuf_out+=sizeof(FL_HEADER);
	pbuf_in+=sizeof(FL_HEADER);
	// do with section
	int sections=pheader->nSection;
	FL_SECTION_INF *psection;
	UFL_CHAR_INDEX *dcharInfo;
	UFL_CHAR_INDEX_EX *dcharinfo_ex;
	unsigned long char1Addr;
	unsigned int numChar;
	if(pheader->magic[0] == 'U'){ // unicode
		if(pheader->magic[1] == 'F')
		{
			psection=(FL_SECTION_INF *)pbuf_in;
			printf("nsections %d \n",sections);
			for(i=0;i<sections;i++,psection++){
				printf("section %d: off:%ld (%#06x->%#06x) \n",i,psection->OffAddr,psection->First,psection->Last);
				// char info index
				dcharInfo=(UFL_CHAR_INDEX *)(buf_in+psection->OffAddr);
				for(j=psection->First;j<=psection->Last;j++){
					dcharInfo->padding=font_swab32(dcharInfo->padding);
					dcharInfo++;
				}
				memcpy(buf_out+psection->OffAddr,buf_in+psection->OffAddr,
					sizeof(UFL_CHAR_INDEX)*(psection->Last-psection->First+1));

				// section
				psection->First=font_swab16(psection->First);
				psection->Last=font_swab16(psection->Last);
				psection->OffAddr=font_swab32(psection->OffAddr);
				
			}
			memcpy(pbuf_out,pbuf_in,sizeof(FL_SECTION_INF)*sections);
		}
		else if(pheader->magic[1] == 'P')
		{
			dcharinfo_ex=(UFL_CHAR_INDEX_EX *)pbuf_in;
			for(i=0;dcharinfo_ex->Code;dcharinfo_ex++,i++){
				dcharinfo_ex->Code=font_swab16(dcharinfo_ex->Code);
				dcharinfo_ex->padding=font_swab32(dcharinfo_ex->padding);
			}
			memcpy(pbuf_out,pbuf_in,sizeof(UFL_CHAR_INDEX_EX)*i);
			printf("total font num: %d \n",i);
		}
		
	}
	//do with index table
	else{ // mbcs 
		if(sections == 1){// have index table	
			// get first useful char info
			i=0;
			do{
				dcharInfo=(UFL_CHAR_INDEX *)pbuf_in;
				if(dcharInfo->padding != 0)
					break;
				memcpy(pbuf_out,(char *)dcharInfo,sizeof(UFL_CHAR_INDEX));
				pbuf_out+=sizeof(UFL_CHAR_INDEX);
				pbuf_in+=sizeof(UFL_CHAR_INDEX);
				i++;
			}while(1);
			printf("start code: %#08x \n",i);
			i=0;
			char1Addr=dcharInfo->le_format.OffAddr+buf_in;
			while(pbuf_in < char1Addr){
				dcharInfo=(UFL_CHAR_INDEX *)pbuf_in;
				dcharInfo->padding=font_swab32(dcharInfo->padding);
				memcpy(pbuf_out,(char *)dcharInfo,sizeof(UFL_CHAR_INDEX));
				pbuf_out+=sizeof(UFL_CHAR_INDEX);
				pbuf_in+=sizeof(UFL_CHAR_INDEX);
				i++;
			}
			numChar=i;
			printf("total char : %d \n",i);
		}
	}

	// wrtie data to file
	if((ret=fwrite(buf_out,1,outfile_size,out_file))==0){
		printf("reach end of file or write error! \n");
		return -1;
	}
	printf("write total size ret = %d \n",ret);
	fclose(in_file);
	fclose(out_file);
	return 0;
}
/*

*/
char *sz_usage=
"usage: ConvertToBigEndian [file-name] [output]\n";

int main(int argc, char *argv[])
{
	unsigned char buf[4*1024];
	unsigned char *pbuf=NULL;
	if(argc < 3){
		printf(sz_usage);
		return 0;
	}
	strcpy(sz_input,argv[1]);
	strcpy(sz_output,argv[2]);
	
	in_file= fopen(sz_input, "rb");
	if (in_file== NULL) {
		printf("open file %s failed.\n",sz_input);
		return -1;
	}
	else{
		printf("open file %s ok.\n",sz_input);
	}
	out_file= fopen(sz_output, "wb+");
	if (out_file== NULL) {
		printf("open file %s failed.\n",sz_output);
		return -1;
	}
	else{
		printf("open file %s ok.\n",sz_output);
	}
	/*
	buf_in=(unsigned char *)malloc(MAX_BUF_SIZE);
	if(pbuf_in == NULL){
		printf("malloc failed\n");
		return -1;
	}
	buf_out=(unsigned char *)malloc(MAX_BUF_SIZE);
	if(pbuf_out == NULL){
		printf("malloc failed\n");
		return -1;
	}
	*/
	// readin data
	pbuf=buf_in;
	infile_size=0;
	int ret;
	while((ret=fread(pbuf,1,READ_SIZE,in_file))!=0){
		//printf("read %d \n",ret);
		pbuf+=ret;
		infile_size+=ret;
	}
	pbuf_out=buf_out;
	printf("total file size: %d \n",infile_size);
		

	to_big_endian();
	return 0;
	
}
