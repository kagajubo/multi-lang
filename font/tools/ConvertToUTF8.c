
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void utf8hcut(const char* infn, const char* outfn)
{
	FILE*	in_fd = NULL;
	FILE*	out_fd = NULL;
	unsigned char bdata;

	in_fd = fopen(infn,"rb");
	if (in_fd == -1)
	{
		fprintf(stderr,"%s failed\n",infn);
		return;
	}

	out_fd = fopen(outfn,"wb+");
	if (out_fd == -1)
	{
		fprintf(stderr,"%s failed\n",outfn);
		fclose(in_fd);
		return;
	}

	fseek(in_fd, 3, SEEK_SET);

	while(1)
	{
		if(1 != fread(&bdata,1,1,in_fd))
			break;
		fwrite(&bdata,1,1,out_fd);
	}


	fclose(in_fd);
	fclose(out_fd);
  	return;	
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("usage:");
		printf("utf8cut [input filename] [output filename]\r\n");
		return 0;
	}

	utf8hcut(argv[1],argv[2]);
	return 0;
}

