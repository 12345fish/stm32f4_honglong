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
#include "can.h"
#include "Uart.h"
#include "Nvic.h"

int main(void)
{
    uint32_t i,j;

    nvic_config();
    uart3_init();

    User_Uart3SendString("  \r\n");
    User_Uart3SendString("��ӭʹ�ñ�����\r\n");                         /* �����ӭ��Ϣ���Դ���         */
    User_Uart3SendString("www.openmcu.com\r\n");                        /* �����ӭ��Ϣ���Դ���         */
    User_Uart3SendString("\r\n");                                       /* �����ӭ��Ϣ���Դ���         */

    User_CAN1Init(); 

    while(1){
        for(i=0;i<30000;i++) {
            for(j=0;j<30;j++);
        }
       //User_Uart3SendString("www.openmcu.com\r\n"); 
       //LED_Turn(GPIO_Pin_6);                                           /* ��˸LED��ʾϵͳ�޿���        */     	   
    }
}


