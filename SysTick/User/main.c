/****************************************Copyright (c)***************************************
**                                 http://www.openmcu.com
**-------------------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-04
** Last Version:        V1.0
** Descriptions:        ����407������ SysTick����
**
**-------------------------------------------------------------------------------------------
** Created by:          FXL
** Last modified Date:  2013-01-04
** Last Version:        V1.0
** Descriptions:        ����407������ SysTick����
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
#include "SysTick.h"

int main(void)
{
    SysTick_Init();        //�δ�ʱ�ӳ�ʼ��
    LEDGpio_Init();        //GPIO��ʼ��

    while (1)
    {
        if(GPIO_ReadOutputDataBit(LED1_GPIO,LED1_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
            LED1_ONOFF(Bit_RESET);
        else
            LED1_ONOFF(Bit_SET);
        Delay(1000);      //��ʱ1��
    }
}

