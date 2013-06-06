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

static void InitBoard(void);

int main(void)
{
    InitBoard();	/* Ϊ����main���������������Щ�����ǽ���ʼ���Ĵ����װ��������� */

	while(1)
	{
//    	DispLogo();		/* ��ʾ����Logo */
    
    	RadioMain();	/* FM������������ */
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
static void InitBoard(void)
{
	/*
		���������ST���еĺ���������ʵ����
		Libraries\CMSIS\Core\CM3\system_stm32f10x.c
    */
	/* ���ð���GPIO��LED GPIO */
	GPIO_Configuration();

	/* �� SysTick_Config()ǰ�������ȵ��� */
	InitButtonVar();

	/* ����systic��Ϊ1ms�ж�,���������
	\Libraries\CMSIS\Core\CM3\core_cm3.h */
    SysTick_Init();
}

