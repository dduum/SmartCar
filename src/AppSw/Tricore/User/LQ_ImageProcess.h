/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��zyf/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.0
�������¡�2020��4��10��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��Hightec4.9.3/Tasking6.3�����ϰ汾
��Target �� TC264DA
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
����iLLD_1_0_1_11_0�ײ����
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_
#define SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_

extern volatile sint16 ECPULSE1;          // �ٶ�ȫ�ֱ���
extern volatile sint16 ECPULSE2;          // �ٶ�ȫ�ֱ���
extern volatile sint32 RAllPulse;         // �ٶ�ȫ�ֱ���

void CameraCar(void);
void TFT_Show_Camera_Info(void);
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW], uint8 imageOut[2][LCDW]);
uint8 GetRoadWide(uint8 imageInput[LCDH][2] ,uint8 imageOut[LCDH]);
uint8 RoadIsNoSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 lineIndex);
void RoadNoSideProcess(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 mode, uint8 lineIndex);
uint8 ImageGetSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2]);

#endif /* SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_ */
