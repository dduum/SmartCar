/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.2 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2023��6��6��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.6�汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_Soft_SPI_H_
#define __LQ_Soft_SPI_H_
#include "include.h"

//                   ��Ƭ��    ����ģ��
#define Soft_SPI_SCK_PIN      P15_8     //SCK
#define Soft_SPI_MISO_PIN     P15_5     //��Ƭ��MISO��ģ���--SDO
#define Soft_SPI_MOSI_PIN     P15_7     //��Ƭ��MOSI��ģ���--SDI
#define Soft_SPI_CS_PIN       P15_2     //CS

#define Soft_SPI_SCK_OUT_H    PIN_Write(Soft_SPI_SCK_PIN, 1)        //��������ߵ�ƽ
#define Soft_SPI_SCK_OUT_L    PIN_Write(Soft_SPI_SCK_PIN, 0)        //��������͵�ƽ

#define Soft_SPI_MISO_IN      PIN_Read(Soft_SPI_MISO_PIN)         //��ȡ�����ϵ�����״̬

#define Soft_SPI_MOSI_OUT_H   PIN_Write(Soft_SPI_MOSI_PIN, 1)       //��������ߵ�ƽ
#define Soft_SPI_MOSI_OUT_L   PIN_Write(Soft_SPI_MOSI_PIN, 0)       //��������͵�ƽ

#define Soft_SPI_CS_OUT_H     PIN_Write(Soft_SPI_CS_PIN, 1)         //��������ߵ�ƽ
#define Soft_SPI_CS_OUT_L     PIN_Write(Soft_SPI_CS_PIN, 0)         //��������͵�ƽ

void Soft_SPI_Init(void);
void Soft_SPI_ReadWriteNbyte(unsigned char *lqbuff, unsigned char len);
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);
void LQ_SPI_Write(unsigned char reg, unsigned char value);
#endif

















