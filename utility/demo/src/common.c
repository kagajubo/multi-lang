#include <string.h>

unsigned long String2Hex(char *pstr)
{
	int len = strlen(pstr);
	unsigned long ulVal = 0;
	unsigned char ucByte = 0;

	int i = 0;
	while(i < len)
	{
		ucByte = pstr[i];
		if(ucByte >= '0' && ucByte <= '9')
			ucByte -= '0';
		else if(ucByte >= 'a' && ucByte <= 'f')
			ucByte = ucByte - 'a' + 10;
		else if(ucByte >= 'A' && ucByte <= 'F')
			ucByte = ucByte - 'A' + 10;
		else
			break;

		ulVal = ulVal << 4;
		ulVal |= ucByte;
		i++;
	}

	return ulVal;
}