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


#include "include.h"


////////////////////////////////////////////////////////////////////////////////

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void Soft_SPI_Init(void)
@����˵����ģ��SPI��ʼ��
@����˵������
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�������
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_Init(void)
{  
    PIN_InitConfig(Soft_SPI_SCK_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(Soft_SPI_MISO_PIN, PIN_MODE_INPUT, 0);
    PIN_InitConfig(Soft_SPI_MOSI_PIN, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(Soft_SPI_CS_PIN, PIN_MODE_OUTPUT, 0);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
@����˵����ģ��SPI��д���ݼ�����
@����˵����uint8_t *buf����ָ��,u16 len����
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�����SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_ReadWriteNbyte(uint8_t *lqbuff, uint8_t len)
{
  uint8_t i;
  
  Soft_SPI_CS_OUT_L;
  Soft_SPI_SCK_OUT_H;
  do
  {
    for(i = 0; i < 8; i++)
    {
      if((*lqbuff) >= 0x80)Soft_SPI_MOSI_OUT_H;        
      else Soft_SPI_MOSI_OUT_L;              
      Soft_SPI_SCK_OUT_L;
      (*lqbuff) <<= 1;            
      Soft_SPI_SCK_OUT_H;     
      (*lqbuff) |= Soft_SPI_MISO_IN;          
    }
    lqbuff++;
  }while(--len);
  Soft_SPI_CS_OUT_H;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
@����˵����SPI���豸��ȡ����
@����˵����unsigned char reg,�豸��ʼ��ַunsigned char* buf,���ݴ�ŵ�ַunsigned short len,��д������ݳ���
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�����SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
{  
  unsigned short i;
  buf[0] = reg | 0x80;  
  Soft_SPI_ReadWriteNbyte(buf, len + 1); 
  for(i=0;i<len;i++)
    buf[i]=buf[i+1];
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@�������ƣ�void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
@����˵����SPI���豸д������
@����˵����unsigned char reg,�豸��ʼ��ַunsigned char* buf,���ݴ�ŵ�ַunsigned short len,��д������ݳ���
@�������أ���
@�޸�ʱ�䣺2022/02/24
@���÷�������
@��    ע����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned char value)
{
    unsigned char buff[2];

    buff[0] = reg & 0x7f;          //�ȷ��ͼĴ���
    buff[1] = value;        //�ٷ�������
    Soft_SPI_ReadWriteNbyte(buff, 2);
}

