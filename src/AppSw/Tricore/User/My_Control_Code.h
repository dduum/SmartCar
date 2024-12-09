/*
 * My_Control_Code.h
 * ���ԭ���洢��CCU6�еĴ���
 *  Created on: 2024��4��21��
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_
#define SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_

#include <My_Key.h>
#include <LQ_PID.h>
#include <LQ_CAMERA.h>
#include <LQ_MotorServo.h>
#include <Platform_Types.h>
#include <My_UART.h>
#include <FuzzyPID.h>
#include <LQ_CCU6.h>
#include <Element_handle.h>
#include <LQ_CCU6.h>
#include <LQ_IIC_Gyro.h>
#include <math.h>

extern float test_out;
extern uint8 Close_Motor_flag;
extern uint8 Speed_up_flag;
extern float k_qz;
extern int PID_flag_Direction;
extern uint8 sp_change;
extern float sp; //��תpϵ��
extern float Tmp_FuzzyOut;
extern char Run_Mode;
extern short Dir_out;
extern float K_Dir;
extern float Image_kp;
extern float Image_kd;
extern int Look_Line;
extern uint8 Key_Value;
extern int change_count;
extern uint8 Select_Mode;
extern float Motor_IncPID;
extern int Servo_duty;
extern uint8 Motor_openFlag;         //���������־
extern uint8 Servo_openFlag;         //���������־
extern short Motor_duty1;
extern short Motor_duty2;
extern int Target_Speed;
extern int Refer_Speed;
extern int max_pix;
extern int min_pix;
extern volatile sint16 LPulse;          // �ٶ�ȫ�ֱ���
extern volatile sint16 YPulse;          // �ٶ�ȫ�ֱ���
extern int Test_err;
extern int Test_err_d;

void Init_System(void);
void Modify_PID(void);
void Key_Control(void);
void Key5_Handle(void);
void Motor_Control(void);
void Servo_Control_PD(void);
void Servo_Control_Fuzzy(void);
int Low_Pass_Filter(short int speedLR,int Feedback_Speed,long int Feedback_Speed_tmp,char s);

#endif /* SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_ */
