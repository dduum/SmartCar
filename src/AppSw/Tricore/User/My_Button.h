/*
 * My_Button.h
 *
 *  Created on: 2024��4��29��
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_BUTTON_H_
#define SRC_APPSW_TRICORE_USER_MY_BUTTON_H_

#define BtnP    0    /* ��� */
#define BtnN    1    /* �Ҽ� */
#define BtnE    2    /* ȷ�ϼ� */
#define BtnR    3    /* ���ؼ� */

#include <LQ_GPIO.h>
#include <LQ_TFT18.h>
#include <stdio.h>
#include <My_Key.h>
#include <My_menu.h>
#include <My_Control_Code.h>

extern int chooseBuf;    //����ѡ��ʱ����������ò˵�
extern int switch_flag;    //ҳ����±�־

int isBtnP();
int isBtnN();
int isBtnE();
int isBtnR();
void Button_Scan(void);

#endif /* SRC_APPSW_TRICORE_USER_MY_BUTTON_H_ */
