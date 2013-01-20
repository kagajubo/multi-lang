#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../_font.h"

#define TRUE 	(1)
#define FALSE 	(0)

#define SELECT_BY_CODELIST_FILE	(1)
#define SELECT_BY_START_END		(2)

#define MAX_BUF_SIZE	(1024*1024*5)
#define READ_SIZE	(32*1024)
FILE *in_fd;
FILE *out_fd;
FILE *code_fd;

//unsigned char *buf_in;
//unsigned char *buf_out;
unsigned char buf_in[MAX_BUF_SIZE];
unsigned char buf_out[MAX_BUF_SIZE];

unsigned char *pbuf_in;
unsigned char *pbuf_out;

#define MAX_CODES	(20000)
unsigned short codesList[MAX_CODES];
unsigned short min_code;
unsigned short max_code;
unsigned short g_start_code;
unsigned short g_end_code;
int	code_num;


char sz_input[128];
char sz_output[128];
char sz_code_file[128];

unsigned long infile_size;
unsigned long outfile_size;


const char *sz_help=
	"SelectCharCode [input-file] [output-file] [mode: 1] [start-code] [end-codde]\n"
	"SelectCharCode [input-file] [output-file] [mode: 2] [in-code-file]\n";


int is_available_code(int mode,unsigned short code)
{
	int i;
	if(mode == SELECT_BY_START_END){
		if((code >= g_start_code) && (code <= g_end_code))
			return TRUE;
		else
			return FALSE;
	}
	else if(mode == SELECT_BY_CODELIST_FILE){
		for(i=0;i<code_num;i++){
			if(codesList[i] == code)
				return TRUE;
		}
		return FALSE;
	}
	else
		return FALSE;
}

int is_code_insessions(FL_SECTION_INF *psecssions,int num,unsigned short code)
{
	int i;
	FL_SECTION_INF *psecssion=psecssions;
	for(i=0;i<num;i++){
		if((code >= psecssion->First) && (code <= psecssion->Last)){
			//printf("code : %#06x, session : %d \n",code, i);
			return i;
		}
		psecssion++; //psecssion +=sizeof(FL_SECTION_INF);
	}
	printf("given codes range not in one of the secssions\n");
	return -1;
}

int is_start_end_in_sesions(FL_SECTION_INF *psecssions,int num) // only for unicode
{
	int i;
	FL_SECTION_INF *psecssion=psecssions;
	for(i=0;i<num;i++){
		if((g_start_code >= psecssion->First) && (g_end_code <= psecssion->Last))
			return i;
		psecssion ++;
	}
	printf("given codes range not in one of the secssions\n");
	return -1;
}

UFL_CHAR_INDEX *get_charinfo(FL_SECTION_INF *sections,int i_sec,UFL_CHAR_INDEX *charinfos,unsigned short code)
{
	FL_SECTION_INF *psection=sections;
	unsigned int offset=0;
	int i;
	for (i=0;i<i_sec;i++){
		offset+=(psection->Last-psection->First-1)*sizeof(UFL_CHAR_INDEX);
	}
}
// for code list file mode
int select_code_unicode(FL_SECTION_INF *sections,int num,
	UFL_CHAR_INDEX *charinfos_out,unsigned int dataStartOff,int font_size)
{
	int ret;
	int i_code;
	FL_SECTION_INF *psection=sections;
	UFL_CHAR_INDEX *dcharInfo;
	UFL_CHAR_INDEX_EX *dcharInfoEx=charinfos_out;
	unsigned int offset;
	unsigned int dataOff=dataStartOff;
	int fontbytes;
	for(i_code=0;i_code<code_num;i_code++){
		if((ret=is_code_insessions(sections,num,codesList[i_code]))<0){
			return -1;
		}
		psection=(FL_SECTION_INF *)(sections+ret);
		offset=psection->OffAddr;
		dcharInfo=(UFL_CHAR_INDEX *)(buf_in+offset+(codesList[i_code]-psection->First)*sizeof(UFL_CHAR_INDEX));
		dcharInfoEx->Code=codesList[i_code];
		dcharInfoEx->le_format.Width=dcharInfo->le_format.Width;
		dcharInfoEx->le_format.OffAddr=dataOff;
		fontbytes=(dcharInfo->le_format.Width+7)/8*font_size;
		memcpy(buf_out+dataOff,buf_in+dcharInfo->le_format.OffAddr,fontbytes);
		//printf("i:%d code:%#06x, width:%d dataOff: %d bytes:%d size:%d\n",i_code,codesList[i_code],dcharInfoEx->le_format.Width,
		//	dcharInfoEx->le_format.OffAddr,fontbytes,font_size);
		dataOff+=fontbytes;
		dcharInfoEx++;
	}
	//last one is zero
	memset(dcharInfoEx,0,sizeof(UFL_CHAR_INDEX_EX));
	memcpy(buf_out+dataOff,buf_in+dcharInfo->le_format.OffAddr,fontbytes);
	
	return dataOff;
}

// for start_end mode
int select_code_unicode_ex(FL_SECTION_INF *sections,int num,
	UFL_CHAR_INDEX *charinfos_out,unsigned int dataStartOff,int font_size)
{
	int ret;
	int i_code;
	FL_SECTION_INF *psection=sections,*psec;
	UFL_CHAR_INDEX *dcharInfo;
	UFL_CHAR_INDEX *dcharInfo_out=charinfos_out;
	unsigned int offset;
	unsigned int dataOff=dataStartOff;
	int fontbytes;
	//section
	psec=(FL_SECTION_INF *)pbuf_out;
	psec->First=g_start_code;
	psec->Last=g_end_code;
	psec->OffAddr=dataOff;
	memcpy(buf_out+sizeof(FL_HEADER),psec,sizeof(FL_SECTION_INF));
	pbuf_out+=sizeof(FL_HEADER);
	// char info 
	for(i_code=0;i_code<code_num;i_code++){
		if((ret=is_code_insessions(sections,num,codesList[i_code]))<0){
			return -1;
		}
		psection=(FL_SECTION_INF *)(sections+ret);
		offset=psection->OffAddr;
		dcharInfo=(UFL_CHAR_INDEX *)(buf_in+offset+(codesList[i_code]-psection->First)*sizeof(UFL_CHAR_INDEX));
		*dcharInfo_out = *dcharInfo;
		dcharInfo_out->le_format.OffAddr=dataOff;
		fontbytes=(dcharInfo->le_format.Width+7)/8*font_size;
		memcpy(buf_out+dataOff,buf_in+dcharInfo->le_format.OffAddr,fontbytes);
		printf("i:%d code:%#06x, width:%d dataOff: %d bytes:%d size:%d\n",i_code,codesList[i_code],dcharInfo_out->le_format.Width,
			dcharInfo_out->le_format.OffAddr,fontbytes,font_size);
		dataOff+=fontbytes;
		dcharInfo_out++;
	}
	//last one is zero
	memset(dcharInfo_out,0,sizeof(UFL_CHAR_INDEX));
	memcpy(buf_out+dataOff,buf_in+dcharInfo->le_format.OffAddr,fontbytes);
	
	return dataOff;
}


int select_code_mbcs_const_width()
{
	printf("ERROR: not yet!!!\n");
	return -1;
}

int select_code_mbcs_nonconst_width()
{
	printf("ERROR: not yet!!!\n");
	return -1;
}
int select_code_by_codelist()
{
	int i;
	int ret;
	// do with header
	FL_HEADER *pheader;
	pheader = buf_in;
	pbuf_in=buf_in;
	if(!((pheader->magic[0] == 'U' || pheader->magic[0] == 'M') &&
		(pheader->magic[1] == 'F') && (pheader->magic[2] == 'L'))){
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
	
	memcpy(pbuf_out,(char *)pheader,sizeof(FL_HEADER));
	pheader=buf_out;
	pheader->magic[WHOLE_CODEPAGE_CONF_INDEX]=USE_PARTIAL_CODEPAGE_FLAG;
	pheader->nSection=0;
	pheader=buf_in;
	
	pbuf_out+=sizeof(FL_HEADER);
	pbuf_in+=sizeof(FL_HEADER);
	// do with section
	int sections=pheader->nSection;
	FL_SECTION_INF *psection;
	UFL_CHAR_INDEX *dcharInfo;
	UFL_CHAR_INDEX_EX *dcharInfoEx;
	unsigned long char1Addr;
	unsigned int numChar;
	unsigned int dataStartOff;
	if(pheader->magic[0] == 'U'){ // unicode
		psection=(FL_SECTION_INF *)pbuf_in;
		dcharInfo = (UFL_CHAR_INDEX *)(pbuf_in+sizeof(FL_SECTION_INF)*pheader->nSection);
		dcharInfoEx=(UFL_CHAR_INDEX_EX *)pbuf_out;

		dataStartOff=sizeof(FL_HEADER)+sizeof(UFL_CHAR_INDEX_EX)*(code_num+1 /* end code ,  0 */);

		if((ret=select_code_unicode(psection,pheader->nSection,dcharInfoEx,dataStartOff,pheader->YSize))<0)
			return -1;

		//remark file size
		pheader=(FL_HEADER *)buf_out;
		pheader->Size=ret;

		if(fwrite(buf_out,1 ,ret,out_fd) != ret){
			printf("write data size: %d failed\n",ret);
			return -1;
		}
		else
			printf("write data size: %d success\n",ret);
	}
	else{ // mbcs 
		if(sections == 1){// have index table	
			if((ret=select_code_mbcs_nonconst_width())<0)
				return -1;
		}
		else{	//const width font, hasn't index table
			if((ret=select_code_mbcs_const_width())<0)
				return -1;
		}
	}
	fclose(out_fd);
	return 0;
}

int select_code_by_start_end()
{
	int i;
	int ret;
	// do with header
	FL_HEADER *pheader;
	pheader = buf_in;
	pbuf_in=buf_in;
	if(!((pheader->magic[0] == 'U' || pheader->magic[0] == 'M') &&
		(pheader->magic[1] == 'F') && (pheader->magic[2] == 'L'))){
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
	
	memcpy(pbuf_out,(char *)pheader,sizeof(FL_HEADER));
	pheader=buf_out;
	pheader->magic[WHOLE_CODEPAGE_CONF_INDEX]=USE_PARTIAL_CODEPAGE_FLAG;
	pheader->nSection=1;
	pheader=buf_in;
	pbuf_out+=sizeof(FL_HEADER);
	pbuf_in+=sizeof(FL_HEADER);
	// do with section
	int sections=pheader->nSection;
	FL_SECTION_INF *psection;
	UFL_CHAR_INDEX *dcharInfo;
	UFL_CHAR_INDEX_EX *dcharInfoEx;
	unsigned long char1Addr;
	unsigned int numChar;
	unsigned int dataStartOff;
	if(pheader->magic[0] == 'U'){ // unicode
		psection=(FL_SECTION_INF *)pbuf_in;
		dcharInfo = (UFL_CHAR_INDEX *)(pbuf_in+sizeof(FL_SECTION_INF)*pheader->nSection);
		dcharInfoEx=(UFL_CHAR_INDEX_EX *)pbuf_out;

		dataStartOff=sizeof(FL_HEADER)+sizeof(FL_SECTION_INF)*1+sizeof(UFL_CHAR_INDEX_EX)*(code_num+1 /* end code ,  0 */);

		if((ret=select_code_unicode(psection,pheader->nSection,dcharInfoEx,dataStartOff,pheader->YSize))<0)
			return -1;
		
		//remark file size
		pheader=(FL_HEADER *)buf_out;
		pheader->Size=ret;

		if(fwrite(buf_out,1 ,ret,out_fd) != ret){
			printf("write data size: %d failed\n",ret);
			return -1;
		}
		else
			printf("write data size: %d success\n",ret);
	}
	else{ // mbcs 
		printf("ERROR : not yet!!!\n");
	}
	fclose(out_fd);
	return 0;
}



int read_in_code_list(char *file)
{
	char buf[4*1024];
	int started=FALSE;
	int index;
	char *pstr=NULL,*pbuf=NULL;
	memset(buf,0,sizeof(buf));
	
	code_fd=fopen(file,"rb");
	if(code_fd==NULL){
		printf("open file: %s failed\n",file);
		return -1;
	}
	
	index =0;
	while(fgets(buf,sizeof(buf),code_fd)){
		if(strchr(buf,'{'))
			started=TRUE;
		else if(strchr(buf,'}'))
			break;

		pbuf=buf;
		while((pstr=strstr(pbuf,"0x"))!= NULL){
			if(sscanf(pstr,"0x%04x",&codesList[index])==1){
				index++;
			}
			else{
				printf("sscanf code failed. \n");
				return -1;
			}
			pbuf=pstr+2;	// jump over "0x"
		}
	}
	code_num=index;
	printf("readin total code: %d \n",index);

	fclose(code_fd);

	return code_num;

}

int build_code_list(unsigned short start, unsigned short end)
{
	unsigned short i;
	for (i=start,code_num=0;i<=end;i++,code_num++){
		codesList[code_num]=i;
	}
	return code_num;
}

int main(int argc, char *argv[])
{
	int mode=0;
	unsigned short start,end;
	if(argc<4){
		printf("1 ");
		printf(sz_help);
		return -1;
	}
	else if(!strcmp(argv[3],"1")){
		if(argc != 6){
			printf("2 ");
			printf(sz_help);
			return -1;
		}
		mode=SELECT_BY_START_END;
		sscanf(argv[4],"0x%02x",&g_start_code);
		sscanf(argv[5],"0x%02x",&g_end_code);
		//g_start_code=atoi(argv[4]);
		//g_end_code=atoi(argv[5]);
		printf("start code: %#06x --> %#06x \n",g_start_code,g_end_code);
		if(g_start_code >= g_end_code){
			printf("given start code isn't larger than end code.\n");
			return -1;
		}
	}
	else if(!strcmp(argv[3],"2")){
		if(argc != 5){
			printf("3 ");
			printf(sz_help);
			return -1;
		}
		mode=SELECT_BY_CODELIST_FILE;
		strcpy(sz_code_file,argv[4]);
	}
	else{
		printf("4 ");
		printf(sz_help);
		return -1;
	}

	strcpy(sz_input,argv[1]);
	strcpy(sz_output,argv[2]);

	if((in_fd = fopen(sz_input,"rb"))==NULL){
		printf("open file : %s failed\n",sz_input);
		return -1;
	}
	if((out_fd = fopen(sz_output,"wb+"))==NULL){
		printf("open file : %s failed\n",sz_output);
		fclose(in_fd);
		return -1;
	}

	////////////// readin original wordstock file
	char *pbuf=buf_in;
	infile_size=0;
	int ret;
	while((ret=fread(pbuf,1,READ_SIZE,in_fd))!=0){
		//printf("read %d \n",ret);
		pbuf+=ret;
		infile_size+=ret;
	}
	fclose(in_fd);
	pbuf_in=buf_in;
	pbuf_out=buf_out;
	printf("total file size: %d \n",infile_size);
	
	///////////////
	if(mode == SELECT_BY_CODELIST_FILE){
		if(read_in_code_list(sz_code_file)< 0){
			goto ___EXIT;
		}
		select_code_by_codelist();
	}
	else if(mode == SELECT_BY_START_END){
		build_code_list(g_start_code,g_end_code);
		select_code_by_start_end();
	}
	
	return 0;

___EXIT:
	fclose(in_fd);
	fclose(out_fd);
	return -1;
}


