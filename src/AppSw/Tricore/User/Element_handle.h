/*
 * Element_handle.h
 *
 *  Created on: 2024��6��3��
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_
#define SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_

#include <Platform_Types.h>
#include <LQ_MotorServo.h>
#include <LQ_CAMERA.h>
#include "My_Control_Code.h"

typedef struct{
    uint8 Curve_flag;               //���
    uint8 Rampway_flag;             //�µ�
    uint8 Zebra;                    //������
    uint8 Crossroad_flag;           //ʮ��·��
    uint8 Straightaway_flag;        //ֱ��
    uint8 Circle_left;              //��Բ��
    uint8 Circle_right;             //��Բ��
    uint8 Obstruction;              //�ϰ���
    uint8 Small_Curve_flag;         //Сs��
}Image_element;

//����ṹ������Ƿ���Ҫʶ��Ԫ�أ��Զ���Ԫ�����ȼ�˳��Ϊ��1.·�� ��2. ��һ�ж��ߵĶ�·�� 3. ��Բ���� 4. ��Բ���� 5. ��·;  6.ʮ��
//ͼ��Ԫ�ؽṹ��
typedef struct
{
    //ʮ�ֱ�־λ
    unsigned char Cross_Identify;
    //Բ����־λ
    unsigned char Circle_left_Identify;
    unsigned char Circle_Right_Identify;
    //·�ϱ�־λ
    unsigned char Obstruction_Identify;
    unsigned char Straightaway_Identify;
    //�����߱�־λ
    unsigned char Zebra_Identify;
    //·�ϱ�־λ
    unsigned char All_Identify;
    //·�ϱ�־λ
    unsigned char Small_Curve_Identify;
    unsigned char Curve_Identify;
}Image_Identify;

//�������ṹ��
typedef struct
{
    sint16 Cross_Pulse;          // ʮ�ֱ������ܼ�����
    sint16 Cross__Pulse_Last;          // ʮ�ֱ�����������

    sint16 Circle_Left_Pulse;          // ��Բ���������ܼ�����
    sint16 Circle_Left_Pulse_Last;          // ��Բ��������������

    sint16 Circle_Right_Pulse;          // ��Բ���������ܼ�����
    sint16 Circle_Right_Pulse_Last;          // ��Բ��������������

    sint16 Obstruction_Pulse;          // �ϰ���������ܼ�����
    sint16 Obstruction_Pulse_Last;          // �ϰ��������������

    sint16 Curve_Pulse;          // ����������ܼ�����
    sint16 Curve_Pulse_Last;          //���������������

    sint16 Zebra_Pulse;         //�����߼�����
    sint16 Zebra_Pulse_Last;     //�����߼�����

    unsigned char Cross_Pulse_Key;            //ʮ�ּ���������
    unsigned char Circle_Left_Pulse_Key;      //��Բ������������
    unsigned char Circle_Right_Pulse_Key;     //��Բ������������
    unsigned char Obstruction_Pulse_Key;      //·�ϼ���������
    unsigned char Curve_Pulse_Key;            //�������������
    unsigned char Zebra_Pulse_Key;            //�������������
}Pulse_Param_t;

extern char keep_beep;
extern char count_beep;
extern Image_element Element1;
extern Image_Identify Identify1;
extern Pulse_Param_t ALLPULSE;
extern uint8 angle_flag;
extern float Angle_in;//��Բ��ʱ�ĽǶ�
extern float Angle_delta;//�Ƕȱ仯ֵ
extern int width[10];
extern unsigned int zhangaiflag;
extern int tmperr;

void Set_NULLflag(void);
void Calcu_Pulse(void);
void Image_IdentifyInit(Image_Identify * Ide);
void My_PulseClean(Pulse_Param_t * PULSE,int element);
void My_PulseCount(Pulse_Param_t * PULSE,int element);
void Image_IdentifyProcess(Image_Identify * Ide);
void Image_CrossProcess(Image_element* Ele);
void Image_LeftCircleProcess(Image_element* Ele);
void Image_RightCircleProcess(Image_element* Ele);
void My_ObstructionProcess(Image_element* Ele);
void Image_ZebraProcess(Image_element* Ele);
void Control_Beep(Image_element* Image_flag);
void My_PulseKey(Pulse_Param_t * PULSE,int element,unsigned char state);
void Image_IdentifySet(Image_Identify * Ide, Image_element* Ele);
void Speed_Decision(Image_element* element);
void Direction_Decision(Image_element* element);
void Straightaway_Process(Image_element* Ele);
void Small_Curve_Process(Image_element* Ele);
void Curve_Process(Image_element* Ele);
void My_ElementClean(Image_element* Ele,int i);
#endif /* SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_ */
