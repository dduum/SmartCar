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

#ifndef __LQ_GPS_BD_H
#define __LQ_GPS_BD_H

#define true 1
#define false 0

typedef struct SaveData
{
  char GPS_Buffer[128];         //��������
  char isGetData;		            //�Ƿ��ȡ��GPS����
  char isParseData;	            //�Ƿ�������
  char UTCTime[11];		          //UTCʱ��
  char latitude[11];		        //γ��
  char N_S[2];		              //N/S
  char longitude[12];		        //����
  char E_W[2];		              //E/W
  char isUsefull;		            //��λ��Ϣ�Ƿ���Ч
} _SaveData;

typedef struct UseData  //ʵ�ʵĲ�ֵ
{
  int lat_err;          //γ�Ȳ�
  int lon_err;          //���Ȳ�
  int lat_centre;       //�����������ĵ�γ��
  int lon_centre;       //�����������ĵ㾭��
  float radius;         //Բ���İ뾶
  float distan_next;    //����һ��Ŀ���ľ���
  float distan_centre;  //��Բ�ĵľ���
  float lat_cent_err;   //�����ĵ�γ�Ȳ�
  float lon_cent_err;   //�����ĵľ��Ȳ�
  float lat_next_err;   //��һ�����γ�Ȳ�
  float lon_next_err;   //��һ����ľ��Ȳ�

} _UseData;

extern _SaveData Save_Data;

void parseGpsBuffer(void);
void BD1202_test(void);


#endif









