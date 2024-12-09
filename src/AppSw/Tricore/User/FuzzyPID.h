/*
 * FuzzyPID.h
 *
 *  Created on: 2024��5��27��
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_FUZZYPID_H_
#define SRC_APPSW_TRICORE_USER_FUZZYPID_H_

#include "include.h"

typedef struct
{
    float kp;                       //PID����kp
    float ki;                       //PID����ki
    float kd;                       //PID����kd
    float imax;                     //PID�����������ֵ
    float integrator;               //pid������
    float qdetail_kp;               //����kp��Ӧ�����е�ֵ
    float qdetail_ki;               //����ki��Ӧ�����е�ֵ
    float qdetail_kd;               //����kd��Ӧ�����е�ֵ

    float detail_kp;                //�������kp
    float detail_ki;                //�������ki
    float detail_kd;                //�������kd
    float qerro;                    //����e��Ӧ�����е�ֵ
    float qerro_c;                  //����de/dt��Ӧ�����е�ֵ
    float e_gradmembership[2];      //����e��������
    float ec_gradmembership[2];     //����de/dt��������
    int e_grad_index[2];            //����e�������ڹ���������
    int ec_grad_index[2];           //����de/dt�������ڹ���������

    float gradSums[7];
    float KpgradSums[7];   //�������kp�ܵ�������
    float KigradSums[7];   //�������ki�ܵ�������
    float KdgradSums[7];   //�������kd�ܵ�������
    float output;          //ģ��PID���
    float kp_error;
    float kd_error;
}Fuzzy_param_t;


//��������ֵ
#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3

extern Fuzzy_param_t Fpid1;
extern const int num_area;
extern float e_membership_values[7];
extern float ec_membership_values[7];
extern float kp_menbership_values[7];
extern float ki_menbership_values[7];
extern float kd_menbership_values[7];
extern int  Kp_rule_list[7][7];
extern int Ki_rule_list[7][7];
extern int  Kd_rule_list[7][7];
//extern int  Fuzzy_rule_list[7][7];
//extern float fuzzyoutput_menbership_values[7];


void FuzzyPID_Init(Fuzzy_param_t * pid);//ģ��PID��ʼ��
void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c);//����e��de/dt�����ȼ��㺯��
void Fuzzy_GetSumGrad(Fuzzy_param_t * pid);//��ȡ�������kp,ki,kd����������
void Fuzzy_GetOUT(Fuzzy_param_t * pid);//�����������kp,kd,ki��Ӧ����ֵ
float Quantization(float maximum,float minimum,float x);//����ӳ�亯��
float Inverse_quantization(float maximum, float minimum, float qvalues);//������ӳ�亯��
float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre,int type);//ģ��PID����ʵ�ֺ���
int Fuzzy_get_KP_Initial(int type);//�ó�ֵ����
int Fuzzy_get_KD_Initial(int type);



#endif /* SRC_APPSW_TRICORE_USER_FUZZYPID_H_ */
