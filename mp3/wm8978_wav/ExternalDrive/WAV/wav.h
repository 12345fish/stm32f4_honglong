#ifndef	__wav_h__
#define __wav_h__

#include "integer.h"

//#define DWORD	unsigned int
//#define WORD	unsigned short
//RIFF WAVE Chunk
typedef struct
{
	char szRiffID[4];     // 'R','I','F','F'
	DWORD dwRiffSize;	  //�����ļ���С - 8
	char szRiffFormat[4];  // 'W','A','V','E'
}	RIFF_HEADER;

//Format Chunk 
typedef struct  
{
   WORD wFormatTag;
   WORD wChannels;
   DWORD dwSamplesPerSec;
   DWORD dwAvgBytesPerSec;
   WORD wBlockAlign;
   WORD wBitsPerSample;
}	WAVE_FORMAT;
typedef struct  
{
   char   szFmtID[4]; // 'f','m','t',' '
   DWORD   dwFmtSize;
   WAVE_FORMAT wavFormat;
} FMT_BLOCK;


//Fact Chunk (optional)
typedef struct 
{
   char   szFactID[4]; // 'f','a','c','t'
   DWORD   dwFactSize;
}	FACT_BLOCK;


//Data Chunk
typedef struct 
{
   char szDataID[4]; // 'd','a','t','a'
   DWORD dwDataSize;
} DATA_BLOCK;

//WAV ͷ�ṹ
typedef struct
{
	RIFF_HEADER	RIFF;
	FMT_BLOCK	RMT;
	FACT_BLOCK	FACT;
	DATA_BLOCK	DATA;

}	WAVE_HEADER;

//WAV ͷ������Ҫ��3����
typedef struct
{
	unsigned short Channels;	//1--��������2--˫����������
	unsigned int SamplesPerSec;	//����Ƶ��
	unsigned short BitsPerSample;//ÿ��������Ҫ��λbit��  
} WAVE;



void WAVE_HeardInfo(unsigned char* buf,WAVE_HEADER* wave_heard);	
void WAVE_Test(void);

#endif
