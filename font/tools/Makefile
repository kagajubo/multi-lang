RM := @rm -Rf

CC ?= gcc
AR ?= ar
CFLAGS ?= -DTRUE="(1==1)" -DFALSE="(1==0)" -D_GNU_SOURCE -g3 -O0 -Wall $(INC)
#LDFLAGS ?= -lpthread -lm


TOOLS = ToCArray ConvertToBigEndian SelectCharCode GetUnicode ConvertToUTF8

tools : $(TOOLS)
	echo "make tools : ToCArray ConvertToBigEndian SelectCharCode GetUnicode ConvertToUTF8"

ToCArray :
	gcc -g ToCArray.c -o $@
	chmod a+x $@
ConvertToBigEndian :
	gcc -g ConvertToBigEndian.c -o $@
	chmod a+x $@
SelectCharCode :
	gcc -g SelectCharCode.c -o $@
	chmod a+x $@
GetUnicode :
	gcc -g GetUnicode.c -o $@
	chmod a+x $@
ConvertToUTF8:
	gcc -g ConvertToUTF8.c -o $@
	chmod a+x $@

clean :
	rm $(TOOLS)
	
