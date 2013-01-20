#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "convert.h"
#include "_font.h"

#define TRUE 	(1)
#define FALSE 	(0)

#define MAX_BUF_SIZE	(1024*1024*5)
#define READ_SIZE	(32*1024)
FILE *in_fd;
FILE *out_fd;


//unsigned char *buf_in;
//unsigned char *buf_out;
unsigned char buf_in[MAX_BUF_SIZE];
unsigned char buf_out[MAX_BUF_SIZE];

unsigned char *pbuf_in;
unsigned char *pbuf_out;

#define MAX_CODES	(5000)
unsigned short codesList[MAX_CODES];
unsigned short min_code;
unsigned short max_code;
int	code_num;

typedef struct _listNode{
	unsigned short code;
	struct _listNode *next;
}ListNode;
typedef ListNode CodeList;

CodeList *g_codeList=NULL;

char sz_input[128];
char sz_output[128];


unsigned long infile_size;
unsigned long outfile_size;


const char *sz_help=
	"MakeCodesTable [input-file] [output-file]\n";


int add_a_code(unsigned short code)
{
	int added;
	ListNode *p=g_codeList;
	ListNode *node,*q;
	node =(ListNode *)malloc(sizeof(ListNode));
	if(node == NULL){
		printf("malloc failed \n");
		return -1;
	}
	
	node->code=code;
	node->next=NULL;
	if(code_num==0){
		g_codeList=node;
		min_code=max_code=code;
	}
	else if(code < min_code){
		node->next=p;
		g_codeList=code;
		
	}	
	else if(code > max_code){
		while(p->next) p=p->next;
		p->next=node;
	}
	else{
		q=p;
		while(p->next && (p->code < code)){
			q=p;
			p=p->next;
		}
		q->next=node;
		node->next=p;
		
	}
	code_num++;

	return 0;
}

int is_added(unsigned short code)
{
	int added = FALSE;
	int i;
	ListNode *p=g_codeList;
	if(code_num==0){
		added=FALSE;
	}
	else if((code > max_code) || (code < min_code)){
		added =FALSE;
	}
	else{
		for(;p;){
			if(p->code == code){
				added = TRUE;
				break;
			}
			p=p->next;
		}
	}
	return added;
		
}

int save_codes_list()
{
	
}

int make_codes_table()
{
	char buf[1024*4];
	int start=FALSE;
	while(fgets(buf,sizeof(buf),in_fd)){
		if(strchr(buf,'{'))
			start=TRUE;
		else if(strchr(buf,'}'))
			break;
		if(start == TRUE){
			
		}
	}
	return 0;
}

/*
*/
int main(int argc, char *argv[])
{
	if(argc<3){
		printf(sz_help);
		return -1;
	}
	strcpy(sz_input,argv[1]);
	strcpy(sz_output,argv[2]);

	if((in_fd=fopen(sz_input,"rb")==NULL){
		printf("open file : %s failed\n",sz_input);
		return -1;
	}
	if((out_fd=fopen(sz_output,"wb+")==NULL){
		printf("open file : %s failed\n",sz_output);
		fclose(in_fd);
		return -1;
	}
	//
	
	//init system
	memset(codesList,0,sizeof(codesList));
	min_code=max_code=0;
	code_num=0;

	//
	make_codes_table();

	return 0;
}

