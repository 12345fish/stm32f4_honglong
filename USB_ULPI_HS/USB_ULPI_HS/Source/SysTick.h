/*******************************************************
*��  �ƣSSTM32F4_SysTickͷ�ļ�
*��  �ߣ����µ���
*��  �ڣ�2012��12��08��
*********************************************************/
#ifndef __STM32F4xx_SYSTICK_H
#define __STM32F4xx_SYSTICK_H

#include "stm32f4xx.h"

extern void delay_init(uint32_t SYSCLK);
extern void delay_ms(uint16_t nms);
extern void delay_us(uint32_t nus);

#endif /*__STM32F4xx_SYSTICK_H */



