/****************************************************************************
* Copyright (C), 2009-2010, www.armfly.com
*
* �ļ���: i2c_gpio.h
* ���ݼ���: gpioģ��i2cģ���ͷ�ļ�
*
* �ļ���ʷ:
* �汾�� ����       ����    ˵��
* 1.0    2009-11-16 armfly  �������ļ�
*
*/

#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include <inttypes.h>

#define I2C_WR	0
#define I2C_RD	1

/* ����I2C������ʼ�ź� */
void i2c_Start(void);

/* ����I2C����ֹͣ�ź� */
void i2c_Stop(void);

/*
  ��I2C���߷���8bit����
*/
void i2c_SendByte(uint8_t _ucByte);

uint8_t i2c_ReadByte(void);

/* ����1��ACKӦ���źţ�ͬʱ�����豸���ص�SDAֵ */
uint8_t i2c_Ack(void);
void i2c_NAck(void);

void i2c_Configuration(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif
