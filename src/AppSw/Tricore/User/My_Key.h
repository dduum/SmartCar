/*
 * My_Key.h
 *
 *  Created on: 2023��12��10��
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_KEY_H_
#define SRC_APPSW_TRICORE_USER_MY_KEY_H_

#include <LQ_STM.h>
#include <LQ_GPIO.h>
#include <LQ_CCU6.h>
#include "My_Button.h"

#define KEY0_Pin P22_3   //K1
#define KEY1_Pin P22_1   //K2
#define KEY2_Pin P22_2   //k3
#define KEY3_Pin P22_0   //K4
#define KEY4_Pin P33_12  //k5

#define GPIO_LOW 0
#define GPIO_HIGH 1
//ȫ�ǵ͵�ƽ����

typedef enum
{
    KEY_CHECK = 0,  //�������״̬
    KEY_COMFIRM,    //����ȷ��״̬
    KEY_UNPRESSED     //�����ͷ�״̬
}keyState_e;        //״̬ö�ٱ���

typedef struct
{
    keyState_e keyState; //����״̬
    uint8 keyFlag;       //�������±�־
}key_t;                //����״̬�ṹ��

extern key_t Key[5];

void Key_Check(int i, key_t *Key, GPIO_Name_t pin);


#endif /* SRC_APPSW_TRICORE_USER_MY_KEY_H_ */
