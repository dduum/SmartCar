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
#include <LQ_CAMERA.h>
#include <LQ_DMA.h>
#include <LQ_GPIO_LED.h>
#include <LQ_TFT18.h>
#include <LQ_MotorServo.h>
#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_MotorServo.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_GPT12_ENC.h>
#include "LQ_MT9V034.h"
#include "LQ_ImageProcess.h"

/**  @brief    ʹ����ʼ��  */
#define ROAD_START_ROW     119

/**  @brief    ʹ�ý�����  */
#define ROAD_END_ROW       0

uint8 ImageSide[LCDH][2];         //���ұ�������
uint8 ImageSide_last[LCDH][2];    //����ͼƬ���ұ�������
uint8 UpdowmSide[2][LCDW];        //���±�������
uint8 Roadwide[LCDH];             //�������

volatile sint16 ECPULSE1 = 0;          // �ٶ�ȫ�ֱ���
volatile sint16 ECPULSE2 = 0;          // �ٶ�ȫ�ֱ���
volatile sint32 RAllPulse = 0;         // �ٶ�ȫ�ֱ���

/*************************************************************************
 *  �������ƣ�void TFT_Show_Camera_Info(void)
 *  ����˵������ʾ����������Ϣ
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��11��18��
 *  ��    ע��
 *************************************************************************/
void TFT_Show_Camera_Info (void)
{
    char txt[16] = "X:";
    TFTSPI_Init(0);               //TFT1.8��ʼ��0:����  1������
    TFTSPI_CLS(u16BLUE);          //����
    sint16 mps = 0, dmm = 0;    // �ٶȣ�m/s,������ֵ
    sint16 pulse100 = 0;
    uint16 bat = 0;

    dmm = (sint16) (RAllPulse * 100 / 579);         // ����512�����������1��5790�����壬��ֵ̫�󣬳���100
    pulse100 = (sint16) (RAllPulse / 100);
    sprintf(txt, "AP:%05d00", pulse100);           //
    TFTSPI_P8X16Str(3, 4, txt, u16RED, u16BLUE);   // ��ʾ����ƫ�����

    NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // ��ȡSTM0 ��ǰʱ�䣬�õ�����
    mps = (sint16) (dmm / (NowTime / 1000));          // �����ٶ�mm/s
    TFTSPI_Road(18, 0, LCDH, LCDW, (unsigned char *)Image_Use); // TFT1.8��̬��ʾ����ͷ�Ҷ�ͼ��
    //TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);
    sprintf(txt, "%04d,%04d,%04d", OFFSET0, OFFSET1, OFFSET2);
    TFTSPI_P8X16Str(0, 5, txt, u16RED, u16BLUE);       // ��ʾ����ƫ�����
    BatVolt       = ADC_Read(ADC7);  // ˢ�µ�ص�ѹ
    bat = BatVolt * 11 / 25;  // x/4095*3.3*100*5.7
    sprintf(txt, "B:%d.%02dV %d.%02dm/s", bat / 100, bat % 100, mps / 1000, (mps / 10) % 100);  // *3.3/4095*3
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLUE);   // �ַ�����ʾ
    // ����Ͷ��������ʾ
    sprintf(txt, "Sv:%04d Rno:%d", ServoDuty, CircleNumber);
    TFTSPI_P8X16Str(1, 7, txt, u16RED, u16BLUE);     // ��ʾ��������1��������1��ֵ
    sprintf(txt, "M1:%04d, M2:%04d ", MotorDuty1, MotorDuty2);
    TFTSPI_P8X16Str(0, 8, txt, u16RED, u16BLUE);     // ���1-2��ֵ
    sprintf(txt, "E1:%04d, E2:%04d ", ECPULSE1, ECPULSE2);
    TFTSPI_P8X16Str(0, 9, txt, u16RED, u16BLUE);     // ������1-2��ֵ
    /*
    sprintf(txt, "Ring num: %d ", CircleNumber);
    TFTSPI_P8X16Str(2, 6, txt, u16GREEN, u16BLACK);
    sprintf(txt, "M:%03d Q:%d J:%d ", MagneticField, TangentPoint, EnterCircle);
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
    */
}
/*************************************************************************
 *  �������ƣ�void CameraCar(void)
 *  ����˵������ų�˫������ٿ���
 -->1.�����㷨���򵥵ķֶα��������㷨����ѧ��ʾ�����㷨��
 2.�����㷨��PID����Ӧ�ÿ����㷨����ѧ��ʾ�����㷨��
 3.�߶��㷨���Ľ�����ȺЭͬ�����㷨��
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��10��28��
 *  ��    ע������2�����
 *************************************************************************/
void CameraCar (void)
{
    // ����ͷ��ʼ��
    CAMERA_Init(50);
    TFTSPI_P8X16Str(2, 3, "LQ 9V034 Car", u16RED, u16GREEN);
    TFTSPI_P8X16Str(1, 5, "K2 Show Video", u16RED, u16GREEN);
    delayms(500);

    // �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
    mutexCpu0TFTIsOk = 0;             // CPU1�� 0ռ��/1�ͷ� TFT
    //CircleNumber = 2;    // ������Ҫ����Բ���ĸ�����

    // ��������Ҫ���ó���⣬�����ǹ̶�ִ�У�
    // �������Ҫ�ɻɹܺ��ⲿ�ж����ʵ��
    // ���������У��ɻɹ���ͨ��Բ������������������ܴ����Ŀ�����
   // OutInGarage(OUT_GARAGE, ReadOutInGarageMode()); // ���Գ��⣬��������������⣬��֮�Ҳ�����
    //OutInGarage(IN_GARAGE,ReadOutInGarageMode());  // �������

    TFTSPI_CLS(u16BLUE);            // ����
    // �м�CPU0,CPU1...������ͬʱ������Ļ��ʾ�������ͻ����ʾ
    mutexCpu0TFTIsOk = 1;             // CPU1�� 0ռ��/1�ͷ� TFT

    RAllPulse = 0;                  // ȫ�ֱ����������������
    NowTime = STM_GetNowUs(STM0);   // ��ȡSTM0 ��ǰʱ��
    while (1)
    {
        LED_Ctrl(LED1, RVS);     // LED��˸ ָʾ��������״̬
        if (Camera_Flag == 2)
        {
            Camera_Flag = 0;     // �������ͷ�ɼ���ɱ�־λ  �����������򲻻��ٴβɼ�����
            Get_Use_Image();     // ȡ����������ʾ����ͼ������
            Get_Bin_Image(2);    // ת��Ϊ01��ʽ���ݣ�0��1ԭͼ��2��3������ȡ
            Bin_Image_Filter();  // �˲������汻Χ�����ݽ����޸�Ϊͬһ��ֵ
            Seek_Road();         // ͨ���ڰ�����������������ƫ��ֵ

            // ��������ƫ��ֵ��ϵ��Խ����Խ�磬��ֵ������ķ�Χ�йأ��˴�Ϊ��160���ң�Ĭ��Ϊ7��
            ServoDuty = Servo_Center_Mid - (OFFSET1 + OFFSET2 + OFFSET2) * 1/7;

            // Բ������������Ϊ��������ֵԽ��˵��Խƫ��ߣ�
            if((OFFSET2 < -300)||(OFFSET2 > 300))
                ServoDuty = Servo_Center_Mid - OFFSET2 / 7;

            ServoCtrl(ServoDuty);     // ���PWM�����ת��

            // SPEED������ʶ���򣬸���Ϊ����
            MotorDuty1 = MtTargetDuty + ECPULSE1 * 4 - (OFFSET1 + OFFSET2 + OFFSET2) / 10;        // ���PWM
            MotorDuty2 = MtTargetDuty - ECPULSE2 * 4 + (OFFSET1 + OFFSET2 + OFFSET2) / 10;        // ˫�����֣���Ҫȥ��abs

            MotorCtrl(MotorDuty1, MotorDuty2);        // ���ֵ������
            // MotorCtrl(2500, 2500); // ���PWM�̶��������
        }
        if(RAllPulse>10000)
            Reed_Init();            // �ɻɹ�GPIO���жϳ�ʼ������,Ϊͣ�������׼��
        if (Game_Over)
        {
            OutInGarage(IN_GARAGE, ReadOutInGarageMode());
        }

    }
}

/*!
  * @brief    ��ȡ���ұ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    imageOut_last   �� ��һ��ͼƬ��������
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2020/6/23 ���ڶ�
  */
uint8 ImageGetSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2])
{
    uint8 i = 0, j = 0;

    //RoadIsNoSide(imageInput, imageOut, ROAD_START_ROW);

    /* �복ͷ����40�� Ѱ�ұ��� */
    imageOut[119][0] = 0;
    imageOut[119][1] = 159;

    for(i = 118; i >= 0; i--)
    {
        imageOut[i][0] = 0;
        imageOut[i][1] = 159;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for(j = imageOut[i+1][0] + 20; j > 0; j--)
        {
            if(!imageInput[i][j])
            {
                imageOut[i][0] = j ;
                break;
            }
        }
        for(j = imageOut[i+1][1] - 20; j < 160; j++)
        {
            if(!imageInput[i][j])
            {
                imageOut[i][1] = j;
                break;
            }
        }
        /* �����߽� ������������ �����Ƿ��Ҷ��� */
//        if(imageOut[i][0] > (LCDW/2 - 10) && imageOut[i][1] >  (LCDW - 5))
//        {
//            /* �Ҷ��ߴ���  �����ұ���Ϊ���б߽�159�������Ϊ*/
//            RoadNoSideProcess(imageInput, imageOut, 2, i);
//
//            if(i > 70)
//            {
//                imageOut[i][0] += 50;
//            }
//            return 1;
//        }
//
//        /* ����ұ߽� ������������ �����Ƿ����� */
//        if(imageOut[i][1] < (LCDW/2 + 10) && imageOut[i][0] <  (5))
//        {
//            /* ���ߴ��� */
//            RoadNoSideProcess(imageInput, imageOut, 1, i);
//
//            if(i > 70)
//            {
//                imageOut[i][1] -= 50;
//            }
//            return 2;
//
//        }
        /*�������鷢��ͻ��*/
//        if((imageOut_last[110][0] - imageOut[110][0]) > 20 && (imageOut_last[110][0] - imageOut[110][0]) < -20)
//        {
//            for(i = 0;i < LCDH;i++)
//            {
//                imageOut[i][0] = imageOut_last[i][0];
//                imageOut[i][1] = 158;
//            }
//        }
//        if(imageOut_last[110][1] - imageOut[110][1] > 20 && imageOut_last[110][1] - imageOut[110][1] < -20)
//        {
//            for(i = 0;i <= LCDH;i++)
//            {
//                imageOut[i][1] = imageOut_last[i][1];
//                imageOut[i][0] = 1;
//            }
//        }
    }
    return 0;
}

/*!
  * @brief    ��ȡ���±���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���Ӿ��복ͷ�Ͻ����п�ʼ���м�����������
  *
  * @see
  *
  * @date     2021/11/30 ���ڶ�
  */
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW], uint8 imageOut[2][LCDW])
{
    uint8 i = 0, j = 0;
    uint8 last = 60;

    imageOut[0][159] = 0;
    imageOut[1][159] = 119;
    /* �����߱ȽϽ����� �ж��Ƿ��� */
    for(i = last; i >= 0; i--)
    {
        if(!imageInput[i][80])
        {
            imageOut[0][80] = i;
            break;
        }
    }

    for(i = last; i < 120; i++)
    {
        if(!imageInput[i][80])
        {
            imageOut[1][80] = i;
            break;
        }
    }

    /* �������� Ѱ�ұ��� */
    for(i = 80-1; i > 0; i--)
    {
        imageOut[0][i] = 0;
        imageOut[1][i] = 119;

        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for(j = imageOut[0][i+1] + 10; j > 0; j--)
        {
            if(!imageInput[j][i])
            {
                imageOut[0][i] = j;
                break;
            }
        }
        for(j = imageOut[1][i+1] - 10; j < 120; j++)
        {
            if(!imageInput[j][i])
            {
                imageOut[1][i] = j;
                break;
            }
        }
    }
    /*�������� Ѱ�ұ���*/
    for(i = 80+1; i < 159; i++)
        {
            imageOut[0][i] = 0;
            imageOut[1][i] = 119;

            /* ���ݱ߽��������� Ѱ�ұ߽� */
            for(j = imageOut[0][i-1] + 10; j > 0; j--)
            {
                if(!imageInput[j][i])
                {
                    imageOut[0][i] = j;
                    break;
                }
            }
            for(j = imageOut[1][i-1] - 10; j < 120; j++)
            {
                if(!imageInput[j][i])
                {
                    imageOut[1][i] = j;
                    break;
                }
            }
        }
    return 0;
}


/*!
  * @brief    ��ȡ�������
  *
  * @param    imageInput �� ���ұ�������
  * @param    imageOut   �� ������ȱ�������
  *
  * @return   �Ƿ���
  *
  * @note     ˼·���ұ��������ȥ���������
  *
  * @see
  *
  * @date     2022/11/29 ���ڶ�
  */
uint8 GetRoadWide(uint8 imageInput[LCDH][2] ,uint8 imageOut[LCDH])
{
    uint8 i = 0;
    for(i = 10;i <= LCDH - 5; i++)
    {
        imageOut[i] = 0;
        if(imageInput[i][1] > imageInput[i][0])
        {
            imageOut[i] = imageInput[i][1] - imageInput[i][0];
        }
        else
        {
            imageOut[i] = 160;
        }
    }
    return 0;
}


/*!
  * @brief    �ж��Ƿ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    lineIndex  �� ��
  *
  * @return   0��û�ж���   1:��߶���  2���ұ߶���  3�� ���Ҷ�����   4������
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
uint8 RoadIsNoSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 lineIndex)
{
    uint8 state = 0;
    uint8 i = 0;
    static uint8 last = 78;
    imageOut[lineIndex][0] = 0;
    imageOut[lineIndex][1] = 159;
    /* �þ���С���ȽϽ����� �ж��Ƿ��� */
    for(i = last; i > 1; i--)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][0] = i;
            break;
        }
    }
    if(i == 1)
    {
        /* ��߽綪�� */
        state = 1;
    }

    for(i = last; i < 159; i++)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][1] = i;
            break;
        }
    }
    if(i == 159)
    {
        /* ���ұ߽綪�� */
        if(state == 1)
        {
            state = 3;
        }
        /* �ұ߽綪�� */
        else
        {
            state = 2;
        }
    }
    if(imageOut[lineIndex][1] <= imageOut[lineIndex][0])
    {
        state = 4;
    }
    return state;
}

/*!
  * @brief    ���ߴ���
  *
  * @param    imageInput �� ��ֵͼ����Ϣ
  * @param    imageOut   �� ��������
  * @param    mode       �� �Ǳ߶��ߣ�   1����߶���  2���ұ߶���
  * @param    lineIndex  �� ��������
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2020/6/24 ������
  */
void RoadNoSideProcess(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 mode, uint8 lineIndex)
{
    uint8 i = 0, j = 0, count = 0;
    switch(mode)
    {
      case 1:
        for(i = imageOut[lineIndex][1]; i > 1; i--)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(!imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = 0;
                    imageOut[lineIndex - count][1] = i;
                    break;
                }
            }
        }
        break;
      case 2:
        for(i = imageOut[lineIndex][0]; i < 159; i++)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(!imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = i;
                    imageOut[lineIndex - count][1] = 159;
                    break;
                }
            }
        }
        break;

    }
}
