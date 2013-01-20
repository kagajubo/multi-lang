CC_PATH := $(shell find $(CURDIR) -maxdepth 5 -type d)
SRC := $(wildcard $(patsubst %,%/*.c,$(CC_PATH)))
OBJ := $(patsubst %.c,%.o,$(SRC))
DEP := $(patsubst %.o,%.d,$(OBJ))
#INC :=
INC := $(patsubst %,-I%,$(CC_PATH))


ECHO := @echo
RM := @rm -Rf

#CROSS ?=
CC ?= $(CROSS)gcc
AR ?= $(CROSS)ar
CFLAGS ?= -DTRUE="(1==1)" -DFALSE="(1==0)" -D_GNU_SOURCE -g3 -O0 -Wall $(INC)
#LDFLAGS ?= -lpthread -lm

###########################################################
###########################################################

#TARGET := lib$(shell basename $(CURDIR)).a
TEST_ENTRY := fontmake

.PHONY : clean fontmake

%.o : %.c
	$(ECHO) -e "\033[33mMaking $<...\033[0m"
	$(CC) $(CFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o $@ -c $<


#$(TARGET) : $(OBJ)
#	$(AR) -r $@ $^

$(TEST_ENTRY) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

ToCArray :
	gcc -g ToCArray.c -o wordstock/$@
	chmod a+x wordstock/$@
ConvertToBigEndian :
	gcc -g ConvertToBigEndian.c -o wordstock/$@
	chmod a+x wordstock/$@

tools : ToCArray ConvertToBigEndian
	echo "make tools : ToCArray ConvertToBigEndian"

# dependency
sinclude $(DEP)

clean:
	$(RM) $(TARGET) $(OBJ) $(DEP)

