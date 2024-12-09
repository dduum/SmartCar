/*
 * FuzzyPID.c
 *
 *  Created on: 2024��5��27��
 *      Author: dduu
 */
#include "FuzzyPID.h"

Fuzzy_param_t Fpid1;

const int  num_area = 8; //�����������
float e_membership_values[7] = {-3,-2,-1,0,1,2,3}; //����e������ֵ
float ec_membership_values[7] = { -3,-2,-1,0,1,2,3 };//����de/dt������ֵ
float kp_menbership_values[7] = { -3,-2,-1,0,1,2,3 };//�������kp������ֵ
float ki_menbership_values[7] = { -3,-2,-1,0,1,2,3 }; //�������ki������ֵ
float kd_menbership_values[7] = { -3,-2,-1,0,1,2,3 };  //�������kd������ֵ
//float fuzzyoutput_menbership_values[7] = { -3,-2,-1,0,1,2,3 };

int  Kp_rule_list[7][7] = { {PB,PB,PB,PB,PB,PB,PB},     //kp�����
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB} };

int  Ki_rule_list[7][7] = { {PB,PB,PB,PB,PB,PB,PB},     //ki�����
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB} };

int  Kd_rule_list[7][7] = { {ZO,ZO,ZO,ZO,ZO,ZO,ZO},     //kd�����
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO} };


/*************************************************************************
*  �������ƣ�void FuzzyPID_Init(Fuzzy_param_t * pid)
*  ����˵����ģ��PID��ʼ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
void FuzzyPID_Init(Fuzzy_param_t * pid)
{
    pid->kp=0;                       //PID����kp
    pid->ki=0;                       //PID����ki
    pid->kd=0;                       //PID����kd
    pid->imax= 100;                   //PID�������ֵ
    pid->integrator=0;                //PID������

    pid->qdetail_kp=0;               //����kp��Ӧ�����е�ֵ
    pid->qdetail_ki=0;               //����ki��Ӧ�����е�ֵ
    pid->qdetail_kd=0;               //����kd��Ӧ�����е�ֵ

    pid->detail_kp=0;                //�������kp
    pid->detail_ki=0;                //�������ki
    pid->detail_kd=0;                //�������kd
    pid->qerro=0;                    //����e��Ӧ�����е�ֵ
    pid->qerro_c=0;                  //����de/dt��Ӧ�����е�ֵ
    pid->output=0;                   //ģ��PID���
}

/*************************************************************************
*  �������ƣ�void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c)
*  ����˵��������e��de/dt�����ȼ��㺯��
*  ����˵����
* @param    pid     ģ��pid����
* @param    erro   pid�������
* @param    erro_c   pid��������
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c)
{     int i=0;
        //����������
        if(erro > e_membership_values[0] && erro < e_membership_values[6])//������Χ��
        {
            for(i = 0; i <num_area-2; i++)
            {
                if(erro >= e_membership_values[i] && erro <= e_membership_values[i+1])//���������ҵ�����������
                {
                    //����������
                    pid->e_gradmembership[0] = -(erro-e_membership_values[i+1])/(e_membership_values[i + 1] - e_membership_values[i]);
                    pid->e_gradmembership[1] = 1+(erro-e_membership_values[i+1])/(e_membership_values[i + 1] - e_membership_values[i]);
                    pid->e_grad_index[0] = i;
                    pid->e_grad_index[1] = i+1;
                    break;
                }
            }
        }
        else
        {       //������С������Χ���߳�������Χ
             if (erro <= e_membership_values[0])
            {
                pid->e_gradmembership[0] = 1;
                pid->e_gradmembership[1] = 0;
                pid->e_grad_index[0] = 0;
                pid->e_grad_index[1] = -1;
            }
            else if (erro >= e_membership_values[6])
            {
                pid->e_gradmembership[0] = 1;
                pid->e_gradmembership[1] = 0;
                pid->e_grad_index[0] = 6;
                pid->e_grad_index[1] = -1;
            }
        }
        //������������
        if (erro_c > ec_membership_values[0] && erro_c < ec_membership_values[6])
    {
        for (i = 0; i < num_area - 2; i++)
        {
            if (erro_c >= ec_membership_values[i] && erro_c <= ec_membership_values[i + 1])
            {
                pid->ec_gradmembership[0] = -(erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);
                pid->ec_gradmembership[1] = 1 + (erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);
                pid->ec_grad_index[0] = i;
                pid->ec_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
            //������С������Χ���߳�������Χ
        if (erro_c <= ec_membership_values[0])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 0;
            pid->ec_grad_index[1] = -1;
        }
        else if (erro_c >= ec_membership_values[6])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 6;
            pid->ec_grad_index[1] = -1;
        }
    }
}
/*************************************************************************
*  �������ƣ�void Fuzzy_GetSumGrad(Fuzzy_param_t * pid)
*  ����˵������ȡ�������kp,ki,kd����������
*  ����˵����
* @param    pid     ģ��pid����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
void Fuzzy_GetSumGrad(Fuzzy_param_t * pid)
{
    int i=0;
    int j=0;
    //��ʼ��PID������������������
    for(i=0; i <7 ; i++)
    {
        pid->KpgradSums[i]=0;
        pid->KigradSums[i]=0;
        pid->KdgradSums[i]=0;
    }
    for(i=0;i<2;i++)
    {
        if(pid->e_grad_index[i] == -1)
        {
            continue;
        }
        for(j=0; j<2;j++)
        {
            if(pid->ec_grad_index[j] != -1)
            {
              int indexKp = Kp_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
              int indexKi = Ki_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
              int indexKd = Kd_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;  //+3��Ŀ���ǽ���-3��+3��������ƫ�Ƶ����顾0��6��

              pid->KpgradSums[indexKp] = pid->KpgradSums[indexKp] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
              pid->KigradSums[indexKi] = pid->KigradSums[indexKi] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
              pid->KdgradSums[indexKd] = pid->KdgradSums[indexKd] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);  //����+=��ԭ������Ϊ�����ж���Ƶ����ͬ��Ӧͬһ��P,�ڹ�����ж�Ӧͬһ��
             }
             else
             {
              continue;
             }
        }
     }
}
/*************************************************************************
*  �������ƣ�void Fuzzy_GetOUT(Fuzzy_param_t * pid)
*  ����˵���������������kp,kd,ki��Ӧ����ֵ
*  ����˵����
* @param    pid     ģ��pid����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
void Fuzzy_GetOUT(Fuzzy_param_t * pid)
{
      for (int i = 0; i < num_area - 1; i++)
    {
        pid->qdetail_kp += kp_menbership_values[i] * pid->KpgradSums[i];
        pid->qdetail_ki += ki_menbership_values[i] * pid->KigradSums[i];
        pid->qdetail_kd += kd_menbership_values[i] * pid->KdgradSums[i];
    }

}
/*************************************************************************
*  �������ƣ�float Quantization(float maximum,float minimum,float x)
*  ����˵��������ӳ�亯��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
float Quantization(float maximum,float minimum,float x)
{
    float qvalues= 6.0 *(x-minimum)/(maximum - minimum)-3;
    return qvalues;
}


/*************************************************************************
*  �������ƣ�float Inverse_quantization(float maximum, float minimum, float qvalues)
*  ����˵����������ӳ�亯��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
float Inverse_quantization(float maximum, float minimum, float qvalues)
{
    float x = (maximum - minimum) *(qvalues + 3)/6 + minimum;
    return x;
}

/*************************************************************************
*  �������ƣ�float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min,
                            float ec_max, float ec_min, float kp_max, float kp_min, float erro,
                            float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,
                            float erro_pre,float errp_ppre,int type)
*  ����˵����ģ��PID����ʵ�ֺ���
*  ����˵����
* @param    pid     ģ��pid����
* @param    e_max   ������ֵ
* @param    e_min   �����Сֵ
* @param    ec_max  �������ֵ
* @param    ec_min  ������Сֵ
* @param    kp_max  kp���ֵ
* @param    kp_min  kp��Сֵ
* @param    erro    ���
* @param    erro_c  ����
* @param    ki_max  ki���ֵ
* @param    ki_min  ki��Сֵ
* @param    kd_max  kd���ֵ
* @param    kd_min  kd��Сֵ
* @param    erro_pre    ��һ�����
* @param    errp_ppre  ����һ�����
* @param    type       ��ֵ
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre,int type)
{
    int num=0;
    int num1=0;
    //ģ����
    pid->qerro = Quantization(e_max,e_min,erro);                //ӳ�䵽����
    pid->qerro_c = Quantization(ec_max,ec_min,erro_c);          //ӳ�䵽����
    Fuzzy_Get_grad_membership(pid,pid->qerro,pid->qerro_c);     //����������
    Fuzzy_GetSumGrad(pid);                                      //����PID���������ܵ�������
    //ģ������
    Fuzzy_GetOUT(pid);                                          //��ȡ������PID����
    //��ģ����
    pid->detail_kp = Inverse_quantization(kp_max,kp_min,pid->qdetail_kp);
    pid->detail_ki = Inverse_quantization(ki_max,ki_min,pid->qdetail_ki);
    pid->detail_kd = Inverse_quantization(kd_max,kd_min,pid->qdetail_kd);   //��ȡ��������PID����
    pid->qdetail_kp = 0;
    pid->qdetail_ki = 0;
    pid->qdetail_kd = 0;

    //��ȡ��ֵ
    num=Fuzzy_get_KP_Initial(0);
    num1=Fuzzy_get_KD_Initial(0);
    //����pid����
    pid->kp =num+pid->detail_kp;
    pid->ki =pid->detail_ki;
    pid->kd =num1+pid->detail_kd;
    //����
    if (pid->kp < 0)
        pid->kp = 0;
    if (pid->ki < 0)
        pid->ki = 0;
    if (pid->kd < 0)
        pid->kd = 0;
    pid->detail_kp=0;
    pid->detail_ki=0;
    pid->detail_kd=0;
    pid->integrator+=erro;//�����ۼ�
    //�����޷�
    if(pid->integrator>100)pid->integrator=100;
    else if(pid->integrator<-100)pid->integrator=-100;

    //��P����
//    pid->kd=0;

    //pd����
    pid->kp_error = pid->kp * erro;
    pid->kd_error = pid->kd * erro_c;
    pid->output = pid->kp_error+pid->kd_error;//pd����
    return pid->output;

}
/*************************************************************************
*  �������ƣ�int Fuzzy_get_KP_Initial(int type)
*  ����˵����ģ��PID���KP��ֵ����
*  ����˵����
* @param    type     ͼ�������ߵ�����
*  �������أ���
*  �޸�ʱ�䣺2023��2��7��
*  ��    ע��
*************************************************************************/
int Fuzzy_get_KP_Initial(int type)
{   int num=0;
    if(type==0)
    {
        num=1;
    }
    else {
        num=0;
    }
    return num;
}

/*************************************************************************
*  �������ƣ�int Fuzzy_get_KD_Initial(int type)
*  ����˵����ģ��PID���Kd��ֵ����
*  ����˵����
* @param    type     ͼ�������ߵ�����
*  �������أ���
*  �޸�ʱ�䣺2023��6��14��
*  ��    ע��
*************************************************************************/
int Fuzzy_get_KD_Initial(int type)
{
    int num=0;
    if(type==0)
    {
        num=1;
    }
    else {
        num=10;
    }
    return num;
}



