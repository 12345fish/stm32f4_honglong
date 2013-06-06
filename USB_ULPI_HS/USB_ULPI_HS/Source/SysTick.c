
/*********************************************************/
#include "SysTick.h"
	 
static uint32_t  My_us=168;//us��ʱ������
static uint32_t  My_ms=168000;//ms��ʱ������
/********************************************************
*��������void delay_init(uint32_t SYSCLK)
*��  ����SYSCLK:ϵͳʱ��  ��λ M  ��168MHz �� 168
*��  �ܣ���ʼ�� systick��ʱ��
*********************************************************/
void delay_init(uint32_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	My_us=SYSCLK/8;		    
	My_ms=(uint16_t)My_us*1000;
}								    
/********************************************************
*��������void delay_ms(u16 nms)
*��  ����nms ��ʱʱ�� n ms
*��  �ܣ�ms��ʱ ע�ⷶΧ��Ҫ����  nms<=0xffffff*8*1000/SYSCLK
*********************************************************/
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t  temp;		   
	SysTick->LOAD=(u32)nms*My_ms;
	SysTick->VAL =0x00;           
	SysTick->CTRL=0x01 ;          
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	SysTick->CTRL=0x00;               
	SysTick->VAL =0X00;               	  	    
}   
/********************************************************
*��������void delay_us(u32 nus)
*��  ����nus ��ʱʱ�� n us
*��  �ܣ�us��ʱ
*********************************************************/	    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*My_us;   		 
	SysTick->VAL=0x00;        
	SysTick->CTRL=0x01 ;      	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	SysTick->CTRL=0x00;               
	SysTick->VAL =0X00;                
}







