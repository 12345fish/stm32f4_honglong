/****************************************Copyright (c)****************************************************
**                                 http://www.openmcu.com
**--------------File Info---------------------------------------------------------------------------------
** File name:           can.c
** Last modified Date:  2012-07-23
** Last Version:        V1.00
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          openmcu
** Created date:        2012-07-23
** Version:             V1.00
** Descriptions:        ��дʾ������
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "can.h"
#include "uart.h"
//#include "led.h"

 CanRxMsg RxMessage;                                                    /*  ȫ�ֱ����������ݽ��ձ���    */

/*********************************************************************************************************
** Function name:       UserCAN1_NVIC_Config
** Descriptions:        ����CAN1�����ж� 
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static void UserCAN1_NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*********************************************************************************************************
** Function name:       User_CAN1Init
** Descriptions:        CAN1��ʼ��,500kbps
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_CAN1Init(void)
{ 
  CAN_InitTypeDef        CAN_InitStructure;                             /*  ����CAN��ʼ���ṹ��         */
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;                       /*  �����������ʼ���ṹ��      */
  GPIO_InitTypeDef       GPIO_InitStructure;                            /*  ����GPIO��ʼ���ṹ��        */

  UserCAN1_NVIC_Config();                                               /*  ����CAN1�����ж�            */

/*-----------��CAN�������õ����ģʽ ----------------------------------*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|                        /*  ʹ�����CAN����ʱ��         */
//                           RCC_APB2Periph_AFIO,ENABLE);    
//    GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);                       /*  ��ӳ��CAN����               */
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Speed =    GPIO_Speed_10MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
    
    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
    
    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

/*-----------CAN��������ʼ�� ------------------------------------------*/
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);

  CAN_DeInit(CAN1);                                                     /*  ��λ��������                */

  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE ;                                 /*  ʹ�����߹���                */
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = ENABLE ;                                 /*  ʹ���Զ��ش�                */
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;                                 /*  ���ȼ���ʲô����            */
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;  //;     CAN_Mode_Normal                    /*  ����Ϊ����ģʽ              */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;                              /*  �������������              */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler = 14;                                 //250
  CAN_Init(CAN1, &CAN_InitStructure);                                   /*  ��������                    */

/*-----------��ʼ���˲�����--------------------------------------------*/
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);                             /*  ��������                    */           
 
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*********************************************************************************************************
** Function name:       User_CANTransmit
** Descriptions:        CAN1��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_CANTransmit(uint16_t val){
  CanTxMsg TxMessage;                                                   /*  ����CAN���Ľṹ��           */
  uint8_t  TransmitMailbox;

  TxMessage.StdId=val&0x7FF;                                            /*  ��̬���֡ID����ֹ��ײ      */
  //TxMessage.ExtId=0x0001;
  TxMessage.RTR=CAN_RTR_DATA;                                           /*  ����֡                      */
  TxMessage.IDE=CAN_ID_STD;                                             /*  ��׼֡                      */
  TxMessage.DLC=2;                                                      /*  ���ݳ�����                  */
  TxMessage.Data[0]=val;                                                /*  ������ݳ�                  */
  TxMessage.Data[1]=(val>>8);                                           /*  ������ݳ�                  */
     
  TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);                        /*  ���Ͳ���ȡ�����            */ 
  while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK));         /*  �ȴ��������                */ 
}

/*********************************************************************************************************
** Function name:       CAN1_RX0_ISR
** Descriptions:        CAN1�����жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void CAN1_RX0_ISR(void)
{
    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {
        uint16_t ch;

        CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);                        /*  ����жϱ�־                */
     
        CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);                         /* ��ȡ����                     */
        ch=(RxMessage.Data[1]<<8)|RxMessage.Data[0];
        User_Uart3SendChar(ch);                                         /* ����1ת������                */          
      
//        LED_Turn(GPIO_Pin_7);                                           /* ָʾCAN���յ�����            */ 
    }
}
/*********************************************************************************************************
**                                        End Of File
*********************************************************************************************************/

