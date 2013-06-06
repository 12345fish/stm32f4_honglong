/*
*
* �ļ���: i2c_gpio.c
* ���ݼ���: ��gpioģ��i2c����, ������STM32ϵ��CPU
*
* �ļ���ʷ:
* �汾�� ����       ����    ˵��
* 1.0    2009-11-15 armfly  �������ļ�
*
*/

#include "stm32f4xx.h"

#include "i2c_gpio.h"

#define GPIO_I2C_PORT  GPIOF
#define RCC_APB2Periph_I2C_PORT  RCC_AHB1Periph_GPIOF
#define I2C_SCL_PIN  GPIO_Pin_1			/* ���ӵ�SCLʱ���ߵ�GPIO */
#define I2C_SDA_PIN  GPIO_Pin_0			/* ���ӵ�SDA�����ߵ�GPIO */

#define I2C_SCL_1()  GPIO_SetBits(GPIO_I2C_PORT, I2C_SCL_PIN);		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(GPIO_I2C_PORT, I2C_SCL_PIN);	/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits(GPIO_I2C_PORT, I2C_SDA_PIN);		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(GPIO_I2C_PORT, I2C_SDA_PIN);	/* SDA = 0 */

#define I2C_SDA_READ()  GPIO_ReadInputDataBit(GPIO_I2C_PORT, I2C_SDA_PIN)

#define GPIO_Speed_MHz GPIO_Speed_50MHz

/*******************************************************************************
	��������i2c_Delay
	��  ��: ��
	��  ��: ��
	����˵����I2C����λ�ӳ٣����400KHz
*/
static void i2c_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 80; i++);
}

/*******************************************************************************
	��������i2c_Start
	��  ��: ��
	��  ��: ��
	����˵��������I2C������ʼ�ź�
*/
void i2c_Start(void)
{
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*******************************************************************************
	��������i2c_Stop
	��  ��: ��
	��  ��: ��
	����˵��������I2C����ֹͣ�ź�
*/
void i2c_Stop(void)
{
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*******************************************************************************
	��������i2c_Stop
	��  ��: _ucByte �����͵�����
	��  ��: ��
	����˵������I2C���߷���8bit����
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		_ucByte <<= 1;
		i2c_Delay();
	}
}

/*******************************************************************************
	��������i2c_Stop
	��  ��: ��
	��  ��: ���ض���������
	����˵������I2C���߶�ȡ8bit����
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*******************************************************************************
	��������i2c_Ack
	��  ��: ��
	��  ��: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
	����˵��������1��ACKӦ���źţ�ͬʱ�����豸���ص�SDAֵ
*/
uint8_t i2c_Ack(void)
{
	uint8_t re;
	GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  /* ��©��� */
//	GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);

	I2C_SDA_1();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	if (I2C_SDA_READ())
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();

    /* Configure in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* ��©��� */
//	GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);
	return re;
}

/*******************************************************************************
	��������i2c_NAck
	��  ��: ��
	��  ��: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
	����˵��������1��ACKӦ���źţ�ͬʱ�����豸���ص�SDAֵ
*/
void i2c_NAck(void)
{
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();
}

/*******************************************************************************
	��������i2c_Configuration
	��  ��: ��
	��  ��: ��
	����˵��������I2C���ߣ�����ģ��IO�ķ�ʽʵ��
*/
void i2c_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_APB2Periph_I2C_PORT, ENABLE);

    /* Configure in output pushpull mode */
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);

//	/* I2C1 SCL PB6 and SDA PB7 pins configuration */
//	GPIO_InitStructure.GPIO_Pin = I2C_SCL_PIN | I2C_SDA_PIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  /* ��©��� */
//	GPIO_Init(GPIO_I2C_PORT, &GPIO_InitStructure);

	/* ��һ��ֹͣ�ź� */
	i2c_Stop();
}

/*******************************************************************************
	��������i2c_Configuration
	��  ��: _Address���豸��I2C���ߵ�ַ
	��  ��: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
	����˵����	���I2C�����豸������ָ����ַ���ݣ�Ȼ���ȡ�豸Ӧ��
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_Configuration();

	/* ������ʼλ */
	i2c_Start();

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	i2c_SendByte(_Address | I2C_WR);

	/* ���ACK */
	ucAck = i2c_Ack();

	/* ����ֹͣλ */
	i2c_Stop();

	return ucAck;
}
