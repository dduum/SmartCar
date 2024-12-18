/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include <LQ_OLED096.h>
#include <LQ_EEPROM.h>
#include <LQ_TFT18.h>
#include <LQ_PID.h>


/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************
*  函数名称：void Test_EEPROM(void)
*  功能说明：测试演示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/

void Test_EEPROM(void)
{
    int i;

//  OLED_Init();
//  OLED_P6x8Str(0,3,"eeprom");         //字符串

    TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
    TFTSPI_CLS(u16BLUE);   //蓝色屏幕
    TFTSPI_P8X16Str(0,0,"eeprom",u16WHITE,u16BLACK);

    unsigned long u32wBuff[24];
    unsigned long u32rBuff[24];

    float f32wBuff[24];
//    float f32rBuff[24];

    /* 赋值 */
    for(i = 0; i < 24; i++)
    {
        u32wBuff[i] = i * 100;
        f32wBuff[i] = i * 2.5f;
    }

    EEPROM_EraseSector(0);
    EEPROM_EraseSector(1);

    TFTSPI_P8X16Str(0,1,"eeporm u write",u16WHITE,u16BLACK);

    /* 写入 */
    EEPROM_Write(0, 0, u32wBuff, 24);

    TFTSPI_P8X16Str(0,3,"eeporm u read",u16WHITE,u16BLACK);

    /* 读出 */
    EEPROM_Read(0, 0, u32rBuff, 24);


    /* 比较 */
    for(i = 0; i < 9; i++)
    {
        if(u32wBuff[i] != u32rBuff[i])
        {
            TFTSPI_P8X16Str(0,5,"u32error",u16WHITE,u16BLACK);
        }
    }

#pragma warning 557         // 屏蔽警告
    while (1);
#pragma warning default     // 打开警告
}


void E2PROM_Read_PID(void)
{
    float PID_Read[16];
    EEPROM_Read(0, 0, (unsigned long*)PID_Read, 16);
    temp_servo_kp=PID_Read[0];
    Servo_Loc_PID.kd=PID_Read[1];
    Motor_Inc_PID.kp=PID_Read[2];
    Motor_Inc_PID.ki=PID_Read[3];
    Motor_Inc_PID.kd=PID_Read[4];
    K_Dir=PID_Read[5];
    Look_Line=(int)PID_Read[6];
    threshold=(int)PID_Read[7];

    sp=PID_Read[8];
    k_qz=PID_Read[9];
    Refer_Speed=(int)PID_Read[10];
    My_Speed_up.Straight_Speed_up=(int)PID_Read[11];
    My_Speed_up.Left_Circle_Speed_up=(int)PID_Read[12];
    My_Speed_up.Curve_Speed_up=(int)PID_Read[13];

}

void E2PROM_Write_PID(void)
{
    float PID_Write[16];

    //擦除扇区
    EEPROM_EraseSector(0);

    PID_Write[0]=temp_servo_kp;
    PID_Write[1]=Servo_Loc_PID.kd;
    PID_Write[2]=Motor_Inc_PID.kp;
    PID_Write[3]=Motor_Inc_PID.ki;
    PID_Write[4]=Motor_Inc_PID.kd;
    PID_Write[5]=K_Dir;
    PID_Write[6]=(float)Look_Line;
    PID_Write[7]=(float)threshold;
    PID_Write[8]=sp;
    PID_Write[9]=k_qz;
    PID_Write[10]=(float)Refer_Speed;
    PID_Write[11]=(float)My_Speed_up.Straight_Speed_up;
    PID_Write[12]=(float)My_Speed_up.Left_Circle_Speed_up;
    PID_Write[13]=(float)My_Speed_up.Curve_Speed_up;

    EEPROM_Write(0, 0, (unsigned long*)PID_Write, 16);
}




