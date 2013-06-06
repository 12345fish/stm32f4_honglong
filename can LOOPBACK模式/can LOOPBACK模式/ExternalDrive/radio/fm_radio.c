/****************************************************************************
*	������ʾ��
*		[TAMPER]��     = ������/����
*		[WAKEUP]��     = ADC��ƽ�л�
*		[USER]��       = ������/�����ر�
*		ҡ����/�¼�    = ��������
*		ҡ����/�Ҽ�    = ������̨/������̨
*		ҡ��OK��       = �Զ���̨/�ֶ���
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.1    2010-09-10 armfly  �������ļ�
*
*/

#include "stm32f4xx.h"
#include <stdio.h>
//#include "systick.h"

#include "button.h"

//#include "tft_lcd.h"
#include "bsp_TEA5767.h"
#include "bsp_WM8978.h"
#include "fm_radio.h"

#define LINE_CAP	18		/* �����м�� */

#define STR_Title	"FM��Ƶ������(TEA5767 + WM8978)"
#define STR_Help1	"[TAMPER]��     = ������/����"
#define STR_Help2	"[WAKEUP]��     = �Զ���̨ADC��ƽ"
#define STR_Help3	"[USER]��       = ������/�����ر�"
#define STR_Help4	"ҡ����/�¼�    = ��������"
#define STR_Help5	"ҡ����/�Ҽ�    = ������̨/������̨"
#define STR_Help6	"ҡ��OK��       = �Զ���̨/�ֶ���̨"

#define MII_MODE /* Mode MII with STM324xG-EVAL  */
#define PHY_CLOCK_MCO

//#define RMII_MODE

RADIO_T g_tRadio;
TEA5767_T g_tTEA;

static void DispTitle(void);
static void DispHelp(void);
static void DispStatus(void);
static void ReadStatus(void);

static void Wm8978_SysInit(void);

/*******************************************************************************
*	�� �� ����RadioMain
*	����˵����������������
*	��    �Σ���
*	�� �� ֵ: ��
*/
void RadioMain(void)
{
	uint8_t ucKeyCode;	/* �������� */
	uint8_t ucRefresh;	/* ˢ��LCD���� */
	
	DispTitle();		/* ��ʾ���� */

	//i2c_Configuration();	/* ����I2C��GPIO */
	//Wm8978_SysInit();

	#if  1	/* ������δ�����Լ��2��оƬ */
		if (i2c_CheckDevice(TEA5767_SLAVE_ADDRESS) != 0)       
		{
			////LCD_DisplayString(20, LINE_CAP, "Don't Find TEA5767");
			//printf("Don't Find TEA5767\r\n");
            //while(1);
		}
		
		if (i2c_CheckDevice(WM8978_SLAVE_ADDRESS) != 0)
		{
			////LCD_DisplayString(20, LINE_CAP, "Don't Find WM8978");
			//printf("Don't Find WM8978\r\n");
            //while(1);
		}
	#endif
	
	DispHelp();		/* ��ʾ������Ϣ */

	/* ��ʼ��ȫ�ֱ��� */
	g_tRadio.ucSpkEn = 0;		/* ��������� */
	g_tRadio.ucMuteOn = 0;		/* �����ر� */
	g_tRadio.ucAutoMode = 1;	/* �Զ���̨ģʽʹ�� */
	g_tRadio.ucVolume = 30;		/* ȱʡ���� */
	g_tRadio.ulFreq = FM_FREQ_MIN;	/* ����/ŷ��(87.5-108M), �ձ�(76-91MHz) */
	g_tRadio.ucAdcLevelSet = ADC_LEVEL_7;

	/* ����WM8978оƬ���������FM�ź� */
	wm8978_Aux2Ear();
	
	wm8978_ChangeVolume(g_tRadio.ucVolume, g_tRadio.ucVolume);	/* ����������������ͬ���� */

	tea5767_Set(g_tRadio.ulFreq, MUTE_OFF, SEARCH_ON, SEARCH_UP, ADC_LEVEL_7);
	
	ucRefresh = 1;

	/* ����������ѭ���� */
	while (1)
	{	g_tRadio.ucAutoMode  = 1;
//		CPU_IDLE();

		/* ����ʱ��ʵʱ��ȡTEA5767��״̬������ѵ���̨��ִ�Ƶ�ʼ��ޣ���ֹͣ */
		if (g_tRadio.ucSearch == 1)
		{
			ReadStatus();	/* ��ȡTEA5767��״̬ */
			if ((g_tTEA.ucReady == 1) || (g_tTEA.ucBandLimit == 1))
			{
				g_tRadio.ucSearch = 0;
				g_tRadio.ucMuteOn = 0;
				wm8978_Mute(0);			/* ȡ������ */				
				ucRefresh = 1;
			}
		}
		
		/* ˢ��״̬�� */
		if (ucRefresh == 1)
		{
			ucRefresh = 0;
			ReadStatus();		/* ��ȡTEA5767��״̬ */
			DispStatus();		/* ��ʾ��ǰ״̬��Ƶ�ʣ������� */
		}
		
		/* �������¼� */
		ucKeyCode = GetKey();
		if (ucKeyCode > 0)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case KEY_DOWN_TAMPER:		/* TAMPER������ */
					if (g_tRadio.ucSpkEn == 1)
					{
						g_tRadio.ucSpkEn = 0;
						wm8978_Aux2Ear();	/* ����WM8978оƬ���������FM�ź� */
					}
					else
					{
						g_tRadio.ucSpkEn = 1;
						wm8978_Aux2Spk();	/* ����WM8978оƬ�����������FM�ź� */
					}
					ucRefresh = 1;	
					break;

				case KEY_DOWN_WAKEUP:		/* WAKEUP������ */
					g_tRadio.ucAdcLevelSet++;
					if (g_tRadio.ucAdcLevelSet > 3)
					{
						g_tRadio.ucAdcLevelSet = 1;
					}
					ucRefresh = 1;					
					break;

				case KEY_DOWN_USER:			/* USER������ */
					if (g_tRadio.ucMuteOn == 1)
					{
						g_tRadio.ucMuteOn = 0;
						wm8978_Mute(0);		/* ������ */
					}
					else
					{
						g_tRadio.ucMuteOn = 1;
						wm8978_Mute(1);		/* ���� */
					}
					ucRefresh = 1;					
					break;
					
				case KEY_DOWN_JOY_UP:		/* ҡ��UP������ */
					if (g_tRadio.ucVolume <= VOLUME_MAX - VOLUME_STEP)
					{
						g_tRadio.ucVolume += VOLUME_STEP;
						wm8978_ChangeVolume(g_tRadio.ucVolume, g_tRadio.ucVolume);
						ucRefresh = 1;
					}
					break;

				case KEY_DOWN_JOY_DOWN:		/* ҡ��DOWN������ */
					if (g_tRadio.ucVolume >= VOLUME_STEP)
					{
						g_tRadio.ucVolume -= VOLUME_STEP;
						wm8978_ChangeVolume(g_tRadio.ucVolume, g_tRadio.ucVolume);
						ucRefresh = 1;
					}					
					break;

				case KEY_DOWN_JOY_LEFT:		/* ҡ��LEFT������ */
					g_tRadio.ulFreq -= FREQ_STEP;
					if (g_tRadio.ulFreq < FM_FREQ_MIN)
					{
						g_tRadio.ulFreq = FM_FREQ_MAX;
					}
					if (g_tRadio.ucAutoMode == 1)
					{
						tea5767_Set(g_tRadio.ulFreq, MUTE_OFF, SEARCH_ON, SEARCH_DOWN, g_tRadio.ucAdcLevelSet);
						g_tRadio.ucSearch = 1;
						g_tRadio.ucMuteOn = 1;
						g_tRadio.ucDirection = SEARCH_DOWN;
						wm8978_Mute(1);		/* ���� */
					}
					else
					{
						tea5767_Set(g_tRadio.ulFreq, MUTE_OFF, SEARCH_OFF, SEARCH_DOWN, g_tRadio.ucAdcLevelSet);							
					}
					ucRefresh = 1;
					break;

				case KEY_DOWN_JOY_RIGHT:	/* ҡ��RIGHT������ */
					g_tRadio.ulFreq += FREQ_STEP;
					if (g_tRadio.ulFreq > FM_FREQ_MAX)
					{
						g_tRadio.ulFreq = FM_FREQ_MIN;
					}
					if (g_tRadio.ucAutoMode == 1)
					{
						tea5767_Set(g_tRadio.ulFreq, MUTE_OFF, SEARCH_ON, SEARCH_UP, g_tRadio.ucAdcLevelSet);
						g_tRadio.ucSearch = 1;
						g_tRadio.ucMuteOn = 1;
						g_tRadio.ucDirection = SEARCH_UP;
						wm8978_Mute(1);		/* ���� */						
					}
					else
					{
						tea5767_Set(g_tRadio.ulFreq, MUTE_OFF, SEARCH_OFF, SEARCH_UP, g_tRadio.ucAdcLevelSet);							
					}						
					ucRefresh = 1;
					break;

				case KEY_DOWN_JOY_OK:		/* ҡ��OK������ */
					if (g_tRadio.ucAutoMode == 1)
					{
						g_tRadio.ucAutoMode = 0;
					}
					else
					{
						g_tRadio.ucAutoMode = 1;
					}
					ucRefresh = 1;	
					break;

				default:
					break;
			}
		}
	}
}

/*******************************************************************************
*	�� �� ����DispTitle
*	����˵������ʾ������
*	��    �Σ���
*	�� �� ֵ: ��
*/
static void DispTitle(void)
{
//	//LCD_Clear(Blue);  			/* ������������ɫ */
//	//LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
//	//LCD_SetTextColor(White);	/* ����������ɫ */
//	
//	////LCD_DisplayString(60, 0, STR_Title);
}

/*******************************************************************************
*	�� �� ����DispHelp
*	����˵������ʾ������Ϣ
*	��    �Σ���
*	�� �� ֵ: ��
*/
static void DispHelp(void)
{
//	uint16_t y;
//
//	//LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
//	//LCD_SetTextColor(White);	/* ����������ɫ */
//	
//	y = LINE_CAP * 2; 	/* �м��(��λ������) */
//	
//	////LCD_DisplayString(20, y, STR_Help1);
//	y += LINE_CAP;
//
//	////LCD_DisplayString(20, y, STR_Help2);
//	y += LINE_CAP;	
//
//	////LCD_DisplayString(20, y, STR_Help3);
//	y += LINE_CAP;
//
//	////LCD_DisplayString(20, y, STR_Help4);
//	y += LINE_CAP;
//
//	////LCD_DisplayString(20, y, STR_Help5);
//	y += LINE_CAP;		
//
//	////LCD_DisplayString(20, y, STR_Help6);
//	y += LINE_CAP;
}
//static void Wm8978_SysInit(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    /* Enable GPIOs clocks */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
//    
//    /* Enable SYSCFG clock */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);  
//    
//    /* Configure MCO (PA8) */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;  
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//    /* MII/RMII Media interface selection --------------------------------------*/
//#ifdef MII_MODE /* Mode MII with STM324xG-EVAL  */
//    #ifdef PHY_CLOCK_MCO
//    /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
//    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_2);
//    #endif /* PHY_CLOCK_MCO */
//    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
//#elif defined RMII_MODE  /* Mode RMII with STM324xG-EVAL */
//    SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
//#endif
//}
/*******************************************************************************
*	�� �� ����ReadStatus
*	����˵������ȡTEA5767��״̬
*	��    �Σ���
*	�� �� ֵ: ��
*/
static void ReadStatus(void)
{
	tea5767_ReadStatus(&g_tTEA);	/* ��ȡTEA5767��״̬ */
	g_tRadio.ulFreq = g_tTEA.ulFreq;
	g_tRadio.ucAdcLevelNow = g_tTEA.ucAdcLevel;
}

/*******************************************************************************
*	�� �� ����DispStatus
*	����˵������ʾ��ǰ״̬��Ƶ�ʣ�������
*	��    �Σ���
*	�� �� ֵ: ��
*/
static void DispStatus(void)
{
	uint16_t y, x1, x2;
	uint8_t buf[80];
	uint8_t ucTemp;

	//LCD_SetTextColor(Yellow);	/* ����������ɫ */
	
	/* ��һ�����ο� */
	//LCD_DrawRect(10, LINE_CAP * 9 + 12, 63, 400 - 20);

	x1 = 20;	/* ״̬����1��X���� */
	x2 = 200;	/* ״̬����2��X���� */	
	y = LINE_CAP * 10;
	
	if (g_tRadio.ucSearch == 1)
	{
		if (g_tRadio.ucDirection == SEARCH_UP)
		{
			sprintf((char *)buf, "Ƶ�� = >>>>>>>>    ");
		}
		else
		{
			sprintf((char *)buf, "Ƶ�� = <<<<<<<<    ");
		}
	}
	else
	{
		sprintf((char *)buf, "Ƶ�� = %d.%d�׺�    ",
			g_tRadio.ulFreq / 1000000, (g_tRadio.ulFreq % 1000000) / 100000);		
	}
	////LCD_DisplayString(x1, y, buf);
	
	sprintf((char *)buf, "���� = %d ", g_tRadio.ucVolume);
//	////LCD_DisplayString(x2, y, buf);	

	y += LINE_CAP;
	
	if (g_tRadio.ucMuteOn == 1)
	{
		////LCD_DisplayString(x1, y, "���� = �� ");
	}
	else
	{
		////LCD_DisplayString(x1, y, "���� = ��");
	}

	if (g_tRadio.ucAutoMode == 1)
	{
		////LCD_DisplayString(x2, y, "��̨ = �Զ�");
	}
	else
	{
		////LCD_DisplayString(x2, y, "��̨ = �ֶ�");
	}

	y += LINE_CAP;
	
	if (g_tRadio.ucAdcLevelSet == 1)
	{
		ucTemp = 5;	
	}
	else if (g_tRadio.ucAdcLevelSet == 2)
	{
		ucTemp = 7;
	}
	else if (g_tRadio.ucAdcLevelSet == 3)
	{
		ucTemp = 10;
	}
	else
	{
		ucTemp = 0;
	}
	sprintf((char *)buf, "ADC  = [%d], %d ",	ucTemp, g_tRadio.ucAdcLevelNow);
	////LCD_DisplayString(x1, y, buf);

	if (g_tRadio.ucSpkEn == 1)
	{
		////LCD_DisplayString(x2, y, "��� = ������");
	}
	else
	{
		////LCD_DisplayString(x2, y, "��� = ����  ");
	}	
		
}
