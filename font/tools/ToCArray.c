#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUF_SIZE	(1024*1024*5)
#define READ_SIZE	(32*1024)

#define OUTPUT_TIMES	(6)
#define LINE_BYTES	(128)

FILE *in_fd;
FILE *out_fd;
unsigned char in_buf[MAX_BUF_SIZE];
char sz_input[128];
char sz_output[128];
char sz_array_name[128];

int to_c_array(unsigned long size)
{
	unsigned long i;
	char tmp[8*1024]={0,};
	char szbyte[16];
	sprintf(tmp,"const unsigned char %s[%d]={",sz_array_name,size);
	if(fwrite(tmp,1,strlen(tmp),out_fd)!=strlen(tmp)){
		printf("write failed !!! \n");
		fclose(out_fd);
		return -1;
	}
	memset(tmp,0,sizeof(tmp));
	for(i=0;i<size;i++){
		if((i%LINE_BYTES)==0){
			strcat(tmp,"\n");
			if(fwrite(tmp,1,strlen(tmp),out_fd)!=strlen(tmp)){
				printf("write failed !!! \n");
				fclose(out_fd);
				return -1;
			}

			memset(tmp,0,sizeof(tmp));
			strcat(tmp,"\t");
		}
		sprintf(szbyte,"%#04x,",in_buf[i]);
		strcat(tmp,szbyte);
			
	}
	
	strcat(tmp,"\n};\n");
	if(fwrite(tmp,1,strlen(tmp),out_fd)!=strlen(tmp)){
		printf("write failed !!! \n");
		fclose(out_fd);
		return -1;
	}

	fclose(out_fd);

	return 0;
}

char *sz_help=
"usage: toCArray [input] [output] [array-name]\n";
int main(int argc,char *argv[])
{
	int ret;
	char tmp[4*1024];
	char *pbuf;
	unsigned int size;
	if(argc<4){
		printf(sz_help);
		return -1;
	}
	strcpy(sz_input,argv[1]);
	strcpy(sz_output,argv[2]);
	strcpy(sz_array_name,argv[3]);

	if((in_fd=fopen(sz_input,"rb"))==NULL){
		printf("open file : %s failed !\n",sz_input);
		return -1;
	}
	printf("open file : %s ok. \n",sz_input);
	if((out_fd=fopen(sz_output,"wb+"))==NULL){
		printf("open file : %s failed !\n",sz_output);
		fclose(in_fd);
		return -1;
	}
	printf("open file : %s ok. \n",sz_output);
	// readin data
	pbuf=in_buf;
	size=0;
	while((ret=fread(pbuf,1,sizeof(tmp),in_fd))>0){
		pbuf+=ret;
		size+=ret;
	}
	printf("read to end or read failed.\n");
	printf("read total size: %d \n",size);
	fclose(in_fd);
	/*
	//MALLOC OUTPUT
	out_buf=malloc(size*OUTPUT_TIMES);
	if(out_buf == NULL){
		printf("malloc for output failed! \n");
		fclose(in_fd);
		fclose(out_fd);
		return -1;
	}
	*/
	to_c_array(size);

	return 0;
}

