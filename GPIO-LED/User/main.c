/****************************************Copyright (c)***************************************
**                                 http://www.openmcu.com
**-------------------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2012-12-28
** Last Version:        V1.0
** Descriptions:        ����407������ LED����
**
**-------------------------------------------------------------------------------------------
** Created by:          FXL
** Last modified Date:  2012-12-28
** Last Version:        V1.00
** Descriptions:        ����407������ LED����
**
**-------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**-------------------------------------------------------------------------------------------
********************************************************************************************/
#include "stm32f4xx.h"
#include "Gpio.h"

void Delay(__IO uint32_t nCount);  //��������

int main(void)
{
  LEDGpio_Init();                  //GPIO�ܽų�ʼ��

  while (1)
  {
      LED4_ONOFF(Bit_SET);         //LED4�øߣ���ƣ� ��ԭ��ͼ���Կ�����LEDΪ�͵�ƽ����
      LED1_ONOFF(Bit_RESET);       //LED1�õͣ����ƣ�
      Delay(0X3FFFFF);             //��ʱ
      LED1_ONOFF(Bit_SET);         //����ͬ��ע��
      LED2_ONOFF(Bit_RESET);
      Delay(0X3FFFFF);
      LED2_ONOFF(Bit_SET);
      LED3_ONOFF(Bit_RESET);
      Delay(0X3FFFFF);
      LED3_ONOFF(Bit_SET);
      LED4_ONOFF(Bit_RESET);
      Delay(0X3FFFFF);
  }
}
/********************************************************************************************
*�������ƣ�void Delay(__IO uint32_t nCount)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵������ʱ����
*******************************************************************************************/
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}


