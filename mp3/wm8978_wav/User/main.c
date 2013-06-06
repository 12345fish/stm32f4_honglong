/****************************************Copyright (c)***************************************
**                                 http://www.openmcu.com
**-------------------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-04
** Last Version:        V1.00
** Descriptions:        ����407������ LCDˢ������
**
**-------------------------------------------------------------------------------------------
** Created by:          FXL
** Last modified Date:  2013-01-04
** Last Version:        V1.00
** Descriptions:        ����407������ LCDˢ������
**
**-------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**-------------------------------------------------------------------------------------------
********************************************************************************************/
#include "stm32f4xx.h"
#include "SysTick.h"
#include "lcd.h"
#include "fm_radio.h"

#include "bsp_wm8978.h"

/*SD ����Ҫ��ͷ�ļ�*/
#include "sdio_sd.h"
#include "ff.h"
#include "ffconf.h"

#include "mad.h"

#include <string.h>

extern unsigned char WAV_NUM;			/*����wav�ļ�����Ŀ*/ 
extern char WAV_Files[100][40];			/*���100���ļ�,*/
void Find_FileName(void);
void WAVE_Test(void);
int MP3_Test(int argc, char *argv[]);

static void InitBoard(void);
void wm8978_I2S_Init(void);
int main(void)
 	{	
	NVIC_InitTypeDef NVIC_InitStructure;
	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#if defined (SD_DMA_MODE)
	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif
    InitBoard();	/* Ϊ����main���������������Щ�����ǽ���ʼ���Ĵ����װ��������� */
	//��������Ŀ¼�ɲ��ŵ�wav�ļ�
	Find_FileName();
	printf("dsfsadfsd\r\nfsadfsdaf\r\n");
	while(1)
	{//	WAVE_Test();
		MP3_Test(WAV_NUM,(char**)WAV_Files);
//		WAVE_Test();
//    	DispLogo();		/* ��ʾ����Logo */
    
//    	RadioMain();	/* FM������������ */
	}
}

/*******************************************************************************
	��������GPIO_Configuration
	��  ��:
	��  ��:
	����˵��������7������Ϊ������ߣ�4��LEDΪ�������

	�������߷��䣺
	USER��     : PG8  (�͵�ƽ��ʾ����)
	TAMPEER��  : PC13 (�͵�ƽ��ʾ����)
	WKUP��     : PA0  (!!!�ߵ�ƽ��ʾ����)
	ҡ��UP��   : PG15 (�͵�ƽ��ʾ����)
	ҡ��DOWN�� : PD3  (�͵�ƽ��ʾ����)
	ҡ��LEFT�� : PG14 (�͵�ƽ��ʾ����)
	ҡ��RIGHT��: PG13 (�͵�ƽ��ʾ����)
	ҡ��SELECT��: PG7 (�͵�ƽ��ʾ����)

	LED���߷��䣺
	LED1 : PF6  (���0����)
	LED2 : PF7  (���0����)
	LED3 : PF8  (���0����)
	LED4 : PF9  (���0����)

*/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��1������GPIOA GPIOC GPIOD GPIOF GPIOG��ʱ��
	   ע�⣺����ط�����һ����ȫ��
	*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOB 
		| RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOF,ENABLE);

	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPUf��λ���������״̬) */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* ��3�����������е�LEDָʾ��GPIOΪ�������ģʽ */
	/* ���ڽ�GPIO����Ϊ���ʱ��GPIO����Ĵ�����ֵȱʡ��0����˻�����LED����
		�����Ҳ�ϣ���ģ�����ڸı�GPIOΪ���ǰ�����޸�����Ĵ�����ֵΪ1 */
//	GPIO_SetBits(GPIOA,  GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_3 | GPIO_Pin_4);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_7 | GPIO_Pin_3 | GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/*******************************************************************************
	��������InitBoard
	��  ��:
	��  ��:
	����˵������ʼ��Ӳ���豸
*/
void InitButtonVar(void);
void uart3_init(void);
void wm8978_I2S_Init(void);
static void InitBoard(void)
{
	SD_Error Status;

	 uart_init();
	/*
		���������ST���еĺ���������ʵ����
		Libraries\CMSIS\Core\CM3\system_stm32f10x.c
    */
	/* ���ð���GPIO��LED GPIO */
	//GPIO_Configuration();

	/* �� SysTick_Config()ǰ�������ȵ��� */
//	InitButtonVar();

	/* ����systic��Ϊ1ms�ж�,���������
	\Libraries\CMSIS\Core\CM3\core_cm3.h */
//    SysTick_Init();

	//SD ��
	do{
		Status = SD_Init();	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	}while(Status!= SD_OK);

	//WM8978
	if (i2c_CheckDevice(WM8978_SLAVE_ADDRESS) == 0)
	{
		wm8978_Cfg(DAC_OFF|1, AUX_ON&0, LINE_OFF|0, SPK_OFF|1, EAR_ON);

		wm8978_ChangeVolume(0x30,0x30);
	}

//	while(1);
	wm8978_I2S_Init();
}
/*********************************************************************************************************
** Functoin name:       WAV_Files
** Descriptions:        ����WAV�ļ�,�ļ������WAV_Files
** input paraments:     ��
** output paraments:    ��    
** Returned values:     ��
*********************************************************************************************************/

unsigned char WAV_NUM=0;			/*����wav�ļ�����Ŀ*/ 
char WAV_Files[100][40];			/*���100���ļ�,*/
void Find_FileName()
{
	DIR dirs;
	FILINFO finfo;
	FATFS fs;

	const XCHAR path[]="";
	unsigned short i;
	FRESULT res=0;

/*���ļ���֧��*/	
#if _USE_LFN
	static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
	finfo.lfname = lfn;
	finfo.lfsize = sizeof(lfn);
#endif


	/*�����ļ�ϵͳ*/
	res = f_mount(0, &fs);
	res=f_opendir(&dirs, path);

	if (res == FR_OK) {								/*�������Ŀ¼�ɹ�*/
		
		while (f_readdir(&dirs, &finfo) == FR_OK){	/*ͨ��dirs�������Ķ��ļ�����֪��ĩβ*/
			
			   	
			if(finfo.fattrib & AM_DIR){	 			/*��Ŀ¼�ͽ�������ѭ��*/
				continue;	
			}
			if (finfo.fattrib & AM_ARC) { 			/*�մ򿪵��ļ������Ǵ浵�ļ�,*/
				
				if(!finfo.fname[0])	 break; 		/*�ļ�β������while*/		
				if(finfo.lfname[0]){				/*���ļ���	*/
					i = (unsigned char )strlen(finfo.lfname);/*���ļ����ĳ��ȣ����֮��õ���׺*/

					if(((finfo.lfname[i-3]=='w') && (finfo.lfname[i-2]=='a') && (finfo.lfname[i-1]=='v'))\
						||((finfo.lfname[i-3]=='W') && (finfo.lfname[i-2]=='A') && (finfo.lfname[i-1]=='V'))){
						strcpy(WAV_Files[WAV_NUM],(const char *)finfo.lfname);
						WAV_NUM++;
					}
			
				}
				else{								/*���ļ���*/
					i = (unsigned char )strlen(finfo.fname);/*���ļ����ĳ��ȣ����֮��õ���׺*/

					if(((finfo.fname[i-3]=='w') && (finfo.fname[i-2]=='a') && (finfo.fname[i-1]=='v'))\
						||((finfo.fname[i-3]=='W') && (finfo.fname[i-2]=='A') && (finfo.fname[i-1]=='V'))){
						strcpy(WAV_Files[WAV_NUM],(const char *)finfo.fname);
						WAV_NUM++;
					}	
				}
			}
		}	
	}
}

