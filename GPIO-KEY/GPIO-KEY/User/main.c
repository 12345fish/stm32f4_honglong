/****************************************Copyright (c)***************************************
**                                 http://www.openmcu.com
**-------------------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-04
** Last Version:        V1.0
** Descriptions:        ����407������ ��������
**
**-------------------------------------------------------------------------------------------
** Created by:          FXL
** Last modified Date:  2013-01-04
** Last Version:        V1.00
** Descriptions:        ����407������ ��������
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

void Delay(int counter);                      //��������

int main(void)
{
    KEYGpio_Init();                           //����IO�ڳ�ʼ��
    LEDGpio_Init();                           //LED��IO�ڳ�ʼ��

    while (1)
    {
        if(KEY_S1_UPDOWM() == Bit_RESET)      //�жϰ���״̬
        {
            Delay(0x8fffff);                  //��ʱȥ��
            if(KEY_S1_UPDOWM() == Bit_RESET)  //�ٴ�ȷ�ϰ���״̬
            {
                if(GPIO_ReadOutputDataBit(LED1_GPIO,LED1_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
                    LED1_ONOFF(Bit_RESET);
                else
                    LED1_ONOFF(Bit_SET); 
            }        
        }

        if(KEY_S2_UPDOWM() == Bit_RESET)
        {
            Delay(0x8fffff);
            if(KEY_S2_UPDOWM() == Bit_RESET)
            {
                if(GPIO_ReadOutputDataBit(LED2_GPIO,LED2_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
                    LED2_ONOFF(Bit_RESET);
                else
                    LED2_ONOFF(Bit_SET); 
            }        
        }

        if(KEY_S3_UPDOWM() == Bit_RESET)
        {
            Delay(0x8fffff);
            if(KEY_S3_UPDOWM() == Bit_RESET)
            {
                if(GPIO_ReadOutputDataBit(LED3_GPIO,LED3_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
                    LED3_ONOFF(Bit_RESET);
                else
                    LED3_ONOFF(Bit_SET); 
            }        
        }

        if(KEY_S4_UPDOWM() == Bit_RESET)
        {
            Delay(0x8fffff);
            if(KEY_S4_UPDOWM() == Bit_RESET)
            {
                if(GPIO_ReadOutputDataBit(LED4_GPIO,LED4_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
                    LED4_ONOFF(Bit_RESET);
                else
                    LED4_ONOFF(Bit_SET); 
            }        
        }

        if(KEY_S5_UPDOWM() == Bit_RESET)
        {
            Delay(0x8fffff);
            if(KEY_S5_UPDOWM() == Bit_RESET)
            {
                if(GPIO_ReadOutputDataBit(LED1_GPIO,LED1_GPIO_PIN) == Bit_SET)     //�жϵƵĵ�ǰ״̬��Ȼ��Ե�ȡ��
                {
                    LED1_ONOFF(Bit_RESET);
                    LED2_ONOFF(Bit_RESET);
                    LED3_ONOFF(Bit_RESET);
                    LED4_ONOFF(Bit_RESET);
                }
                else
                {
                    LED1_ONOFF(Bit_SET);
                    LED2_ONOFF(Bit_SET);
                    LED3_ONOFF(Bit_SET);
                    LED4_ONOFF(Bit_SET);
                }
            }
        }
    }
}

void Delay(int counter)
{
    while(counter--);
}



