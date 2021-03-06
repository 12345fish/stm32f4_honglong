/****************************************Copyright (c)***************************************
**                                 http://www.openmcu.com
**-------------------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2012-12-28
** Last Version:        V1.0
** Descriptions:        红龙407开发板 外部中断测试
**
**-------------------------------------------------------------------------------------------
** Created by:          FXL
** Last modified Date:  2012-12-28
** Last Version:        V1.0
** Descriptions:        红龙407开发板 外部中断测试
**
**-------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**-------------------------------------------------------------------------------------------
********************************************************************************************/
#include "stm32f4xx.h"
#include "Exti.h"
#include "Gpio.h"

int main(void)
{
    LEDGpio_Init();                         //LED灯IO口初始化
    EXTILine0_Config();                     //初始化中断线0 既按键S2
    EXTILine6_Config();                     //初始化中断线6 即按键S5
    
    EXTI_GenerateSWInterrupt(EXTI_Line0);   //用软中断 试一下配置
    
    while (1)
    {
    }
}





