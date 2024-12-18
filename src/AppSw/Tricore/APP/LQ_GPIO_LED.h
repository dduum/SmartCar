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
#ifndef __LQ_LED_H_
#define __LQ_LED_H_


//定义模块号
typedef enum
{
    LED0=0,  //核心板上LED0
    LED1=1,  //核心板上LED1
    LED2=2,  //母板上LED0
    LED3=3,   //母板上LED1
    LEDALL=4
} LEDn_e;

typedef enum
{
    ON=0,  //亮
    OFF=1, //灭
    RVS=2, //反转
}LEDs_e;

//定义的管脚要对应实际按键
#define LED0p      P10_6   //核心板上LED0 翠绿
#define LED1p      P10_5   //核心板上LED1 蓝灯
#define LED2p      P20_6   //母板上LED0
#define LED3p      P20_7   //母板上LED1

#define Beep       P33_8   //蜂鸣器

#define Beep_on PIN_Write(Beep,1)
#define Beep_off PIN_Write(Beep,0)
#define Beep_rev PIN_Reverse(Beep)

/*********************** UART功能函数 **************************/
//初始化
void GPIO_LED_Init(void);
void GPIO_Beep_Init(void);
void Beep_Ctrl(char sta);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_GPIO_LED(void);


#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
