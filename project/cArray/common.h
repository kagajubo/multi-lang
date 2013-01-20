#ifndef ___COMMON_H__
#define ___COMMON_H__

#define DEBUG_1004A
#ifdef DEBUG_1004A
#define FM_DBG(fmt...)	\
	do{	\
		printf("DEBUG::");	\
		printf(fmt);	\
		}while(0)
#define FM_ERR(fmt...)	\
	do{	\
		printf("ERROR::");	\
		printf(fmt);	\
		}while(0)
#else
#define FW_DBG(fmt...)
#define FW_ERR(fmt...)
#endif

#endif

