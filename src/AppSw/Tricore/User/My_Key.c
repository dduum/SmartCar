
/*
 * My_Key.c
  *  ����: ͨ����ʱ��STM�жϼ�ⰴ���Ƿ񰴼���֧������������ֻ����Ӧһ�Σ���ʱ����Ϊ20ms���պ�����ȥ������ʹ��CCU6��CH1ͨ�����ж�ʱ
 *  Created on: 2023��12��10��
 *      Author: dduu
 */

#include "My_Key.h"

//�����жϷ�������LQ_CCU6�� ������ void CCU60_CH0_IRQHandler(void)

key_t Key[5]; //�洢Key��״̬

//ÿ20ms����һ�ζ�ʱ�ж��У���ⰴ��״̬
void Key_Check(int i, key_t *Key, GPIO_Name_t pin)
{
    switch( Key[i].keyState )
    {
        case KEY_CHECK:
            // �����͵�ƽ�����밴��ȷ��״̬
            if(PIN_Read(pin) == GPIO_LOW)
            {
                Key[i].keyState = KEY_COMFIRM;
            }
        break;

        case KEY_COMFIRM:
            if(PIN_Read(pin) == GPIO_LOW)
            {
                //�����͵�ƽ������ȷʵ���£�������־λ��1�������밴���ͷ�״̬
                Key[i].keyState = KEY_UNPRESSED;
                Key[i].keyFlag = 1;
            }
            //�����ߵ�ƽ�������Ǹ����źţ����س�ʼ״̬
            else
            {
                Key[i].keyState = KEY_CHECK;
            }
        break;

        case KEY_UNPRESSED:
            if(PIN_Read(pin) == GPIO_HIGH)
            {
             // �����ߵ�ƽ��˵�������ͷţ����س�ʼ״̬
             Key[i].keyState = KEY_CHECK;
            }
        break;
    }
}



