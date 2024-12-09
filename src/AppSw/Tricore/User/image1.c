/*
 * newimage.c
 *
 *  Created on: 2024��5��14��
 *      Author: Macation
 */
#include "image1.h"

#define Image_downLine 110
#define Image_upLine 15
#define central_length Image_downLine-Image_upLine

int Image_centralLine[central_length];//��ű��ߵ�����
float a=0;
int cout=0;
int qulv_value;
int Image_Lefthand_Line[Array_Length][2];//�����ȡ��ı������������
int Image_Righthand_Line[Array_Length][2];
int newImage_Lefthand_Line[Array_Length][2];
int newImage_Righthand_Line[Array_Length][2];
unsigned char Image_Line[120][160];//��ű��ߵ����飬0��1��ʾ
int Left_num=Array_Length;
int Right_num=Array_Length;
int block_size=7;
int Servo_Loc_error;  //�������
int Servo_QZ_error;   //ǰհ���
int Servo_All_error;  //�ϳ�ƫ��
int image_error[6];
uint8 image_flag;
float currentslope=0;
float currentslope2=0;
double previous_slope = 0, current_slope = 0,sum=0;
int current_state1=0;
int current_state2=0;
int guaidian1[Array_Length][2];
int guaidian2[Array_Length][2];
int guaidiannum1=0;
int shiziflag=0;
int threshold=80;
int qvlv_value=0;
int qulv_jin;
int qulv_yuan;
int qvlv_jin_left;
int qvlv_jin_right;
int qvlv_yuan_left;
int qvlv_yuan_right;
int qvlv_quanju_right,qvlv_quanju_left=0;
//�洢��ǰ������ǰ����ǰ���ķ�������
int dir_front[4][2]=   {{0,-1},
                        {1,0},
                        {0,1},
                        {-1,0}};
int dir_frontleft[4][2]=   {{-1,-1},
                            {1,-1},
                            {1,1},
                            {-1,1}};
int dir_frontright[4][2]=   {{1,-1},
                            {1,1},
                            {-1,1},
                            {-1,-1}};
int crossflag=0;
int shizi=0;
int flag=0;
int zuodianxian,youdianxian=0;
/*************************************************************************
*  �������ƣ�int Image_Threshold_Line(int Y)
*  ����˵�����Ե�������ֵ
*  �������أ���
*  �޸�ʱ�䣺2023��6��15��
*  ��    ע��
*************************************************************************/
int Image_Threshold_Line(int Y)
{
    signed int i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    int MinValue, MaxValue;
    int Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for(i=0;i<MT9V034_IMAGEW;i++)
    {
        HistoGram[Image_Data[Y][i]]++;
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0;
            MaxValue--)
        ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
        return MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)
        return MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  ��������

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++) {
        Pixelshortegral += HistoGram[j] * j;        //�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++) {
        PixelBack = PixelBack + HistoGram[j];     //ǰ�����ص���
        PixelFore = Amount - PixelBack;           //�������ص���
        OmegaBack = (float) PixelBack / Amount;   //ǰ�����ذٷֱ�
        OmegaFore = (float) PixelFore / Amount;   //�������ذٷֱ�
        PixelshortegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //�����Ҷ�ֵ
        MicroBack = (float) PixelshortegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float) PixelshortegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore)
                * (MicroBack - MicroFore);   //������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
                {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //���������ֵ;
}
/*************************************************************************
*  �������ƣ�void Image_Frameprocessing(void)
*  ����˵����
*  ����˵�������ڿ�
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*************************************************************************/
void Image_Frameprocessing(void)
{
    for(int i=0;i<LCDH;i++)
    {
        Image_Use[i][3]=0;
        Image_Use[i][LCDW-3]=0;
    }
}

/*************************************************************************
*  �������ƣ�int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
*  ����˵����
*  ����˵���������Թ�Ѳ��
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*************************************************************************/
int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//�����һ��
    int step = 0, dir = 0, turn = 0;//���ߵĲ�������ǰ����ת��Ĵ���
    while(step<num && half<X && X<LCDW-half-1 && half< Y && Y <LCDH-1 && turn<4)//ѭ����ֹ��������Խ����ת�����С��4����ֹԭ�ش�ת
    {
        int local_thres = 0; //����Ӧ��ֵ
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += Image_Use [Y+dy][X+dx];//��������ֵ�ۼ�
            }
        }
        local_thres /= block_size * block_size;//��ֵȡƽ��
        local_thres -= clip_value;//��ֵ���ü��������Լ���
        int front_value = Image_Use[Y+dir_front[dir][1]][X+dir_front[dir][0]];//��ǰ����ǰ���ĵ������ֵ��
        int frontleft_value = Image_Use[Y+dir_frontleft[dir][1]][X+dir_frontleft[dir][0]];//��ǰ������ǰ���ĵ������ֵ
        if(front_value<local_thres)//ǰ������Ϊ�ڣ�����ת��ת�����+1
        {
            dir=(dir + 1) % 4;
            turn++;
        }
        else if(frontleft_value<local_thres)//��ǰ������Ϊ�ڣ�ֱ�ߣ�ת�������0������+1
        {
            Y+=dir_front[dir][1];
            X+=dir_front[dir][0];
            Image_Lefthand_Line[step][0]=X;
            Image_Lefthand_Line[step][1]=Y;
            if(X==4)zuodianxian++;
            Image_Line[Y][X]=1;
            step++;
            turn=0;
        }
        else
        {
            Y += dir_frontleft[dir][1];
            X += dir_frontleft[dir][0];
            Image_Lefthand_Line[step][0]=X;
            Image_Lefthand_Line[step][1]=Y;
            if(X==4)zuodianxian++;
            Image_Line[Y][X]=1;
            dir=(dir + 3) % 4;
            step++;
            turn=0;
        }
    }
    return step;
}
/*************************************************************************
*  �������ƣ�int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
*  ����˵����
*  ����˵���������Թ�Ѳ��
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*************************************************************************/
int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//�����һ��
    int step = 0, dir = 0, turn = 0;//���ߵĲ�������ǰ����ת��Ĵ���
    while(step<num&& half<X && X<LCDW-half && half< Y && Y <LCDH-1 && turn<4)//ѭ����ֹ��������Խ����ת�����С��4����ֹԭ�ش�ת)
    {
        int local_thres = 0; //����Ӧ��ֵ
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += Image_Use [Y+dy][X+dx];//��������ֵ�ۼ�
            }
        }
        local_thres /= block_size * block_size;//��ֵȡƽ��
        local_thres -= clip_value;//��ֵ���ü��������Լ���
        int front_value = Image_Use[Y+dir_front[dir][1]][X+dir_front[dir][0]];//��ǰ����ǰ���ĵ������ֵ��
        int frontright_value=Image_Use[Y+dir_frontright[dir][1]][X+dir_frontright[dir][0]];
        if(front_value<local_thres)
        {
            dir = (dir + 3) % 4;
            turn++;
        }
        else if(frontright_value < local_thres)
        {
            Y+=dir_front[dir][1];
            X+=dir_front[dir][0];
            Image_Righthand_Line[step][0]=X;
            Image_Righthand_Line[step][1]=Y;
            if(X==156)youdianxian++;
            Image_Line[Y][X]=1;
            step++;
            turn=0;
        }
        else
        {
            Y += dir_frontright[dir][1];
            X += dir_frontright[dir][0];
            Image_Righthand_Line[step][0]=X;
            Image_Righthand_Line[step][1]=Y;
            if(X==156)youdianxian++;
            Image_Line[Y][X]=1;
            dir=(dir + 1) % 4;
            step++;
            turn=0;
        }
    }
    return step;
}
/*************************************************************************
*  �������ƣ�void Image_SeedGrowing(void)
*  ����˵���������������������ұ��ߺ���
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*
*************************************************************************/
void Image_SeedGrowing(void)
{
    cout=0;
    zuodianxian=0;
    youdianxian=0;

    threshold = GetOSTU(Image_Use);  //�����ֵ

    int X1=Image_findSeedBegin(threshold);
    int Y1=Image_downLine;
    int X2=X1;
    int Y2=Y1;

    memset(Image_Line,0,sizeof(Image_Line));
    memset(Image_Lefthand_Line,0,sizeof(Image_Lefthand_Line));
    memset(Image_Righthand_Line,0,sizeof(Image_Righthand_Line));
    memset(guaidian1,0,sizeof(guaidian1));
    memset(guaidian2,0,sizeof(guaidian2));
    for(;X1>0;X1--)//Ѱ�������
    {
        if(Image_Use[Y1][X1-1]<threshold)break;
    }
    Left_num=Array_Length;
    Left_num=Image_Findline_Lefthand(X1,Y1,Left_num,block_size,0);
    for(;X2<LCDW-1;X2++)//Ѱ�������
    {
        if(Image_Use[Y2][X2+1]<threshold)break;
    }
    Right_num=Array_Length;
    Right_num=Image_Findline_Righthand(X2,Y2,Right_num,block_size,0);
    int leftstart;
    for(leftstart=0;leftstart<Left_num;leftstart++)
    {
        if(Image_Lefthand_Line[leftstart][1]==60)break;
    }
    if(Left_num>60)
    {
        for(int j=0;j<79;j++)
        {
            Image_Line[Image_Lefthand_Line[leftstart][1]][j]=1;
            currentslope=linearRegression(Image_Lefthand_Line, leftstart-10, leftstart+10);
//            currentslope=calculateCurvature(Image_Lefthand_Line,leftstart,Left_num);
        }
    }
    int rightstart;
    for(rightstart=0;rightstart<Right_num;rightstart++)
    {
        if(Image_Righthand_Line[rightstart][1]==Image_Lefthand_Line[leftstart][1])break;
    }
    if(Right_num-rightstart>20&&rightstart>10)
    {
        for(int s=159;s>80;s--)
        {
            Image_Line[Image_Righthand_Line[rightstart][1]][s]=1;
            currentslope2=linearRegression(Image_Righthand_Line, rightstart-10, rightstart+10);
//            currentslope2=calculateCurvature(Image_Righthand_Line,rightstart,Right_num);
        }
    }
    current_state1=find(currentslope);
    current_state2=find(currentslope2);
    flag=0;
    leftxielv(leftstart,rightstart);
    Image_IdentifyProcess(&Identify1);//ʶ����
    Image_IdentifySet(&Identify1, &Element1);//����ʶ�����ȼ�
    Image_seedCentralline(X1,X2);//������
    qulv();

//    if(current_state1==1&&current_state2==1)
//    {
//         Beep_on;
//    }
}
/*************************************************************************
*  �������ƣ�int Image_findSeedBegin(Image_element* Ele, int thres)
*  ����˵���������ӵ���ʼ�㺯��
*  �������أ���
*  �޸�ʱ�䣺2023��4��23��
*  ��    ע��
*************************************************************************/
int Image_findSeedBegin(int thres)
{
    int i=0;
    for(i=79;i>2;i--)
    {
        if(Image_Use[Image_downLine][i-1]>thres && Image_Use[Image_downLine][i]>thres
                && Image_Use[Image_downLine][i+1]>thres)
        {
            return i;
        }
    }
    for(i=81;i<MT9V034_IMAGEW -2;i++)
    {
        if(Image_Use[Image_downLine][i-1]>thres && Image_Use[Image_downLine][i]>thres
                && Image_Use[Image_downLine][i+1]>thres)
        {
            return i;
        }
    }

    return LCDW/2;
}
/*************************************************************************
*  �������ƣ�void Image_seedCentralline(int X,int Y)
*  ����˵���������������������ұ��ߺ���
*  �������أ���
*  �޸�ʱ�䣺2024��5��15��
*  ��    ע��
*
*************************************************************************/
void Image_seedCentralline(int X1,int X2)
{
    int  i,j;
    //ɨ�����ʱÿһ�е����
    int LeftStart=X1+5;
    int RightStart=X2-5;
    int Left[central_length]={2};
    int Right[central_length]={LCDW-4};
    int qidian;
    for(i=Image_downLine-1;i>Image_upLine;i--)//������������ĵ�һ������Image_downLine��һ�У�����ȡ��ҲҪ����һ��
    {
        //��ֹ��㳬��ͼ��Χ
        if(RightStart>LCDW-4)
        {
            RightStart=LCDW-4;
        }
        if(LeftStart<2)
        {
            LeftStart=2;
        }
        //��ױ��֣�����ڸ���δ�ҵ�����㣬��ͬ��һ��ѭ���Ľ������һ��
//        if(i<Image_downLine-1)
//        {
//            Left[i-Image_upLine]=Left[i+1-Image_upLine];
//            Right[i-Image_upLine]=Right[i+1-Image_upLine];
//
//        }
        //ɨ���ߣ��������һ�е�����������Ӹ��������Բ����м�㿪ʼ������
        for(j=LeftStart;j > 0;j--)
        {
            if(Image_Line[i][j]==1)//�ҵ������
            {
                Left[i-Image_upLine]=j;

                break;
            }
        }
        //ɨ���ߣ�ͬ��������
        for(j=RightStart;j< LCDW-1;j++)
        {
            if(Image_Line[i][j]==1)//�ҵ������
            {
                Right[i-Image_upLine]=j;

                break;
            }
        }
//        //�滮��һ�ε����
        qidian=(Left[i-Image_upLine]+Right[i-Image_upLine])/2;
        LeftStart=qidian+10;
        RightStart=qidian-10;
//        if(Element1.Crossroad_flag!=0)
//        {
//            LeftStart=80;
//            RightStart=80;
//        }
    }

        for(i=0;i<central_length;i++)
        {
            Image_centralLine[i]=(Left[i]+Right[i])/2;
        }

    //��ͼ���л�������
    for(i=Image_downLine-1;i>Image_upLine-1;i--)
    {
        j=Image_centralLine[i-Image_upLine];
        //�Ӵ֣��������ص�
        Image_Line[i][j]=1;
        Image_Line[i][j-1]=1;

    }
}
/*************************************************************************
*  �������ƣ�void Image_seedCentralline(int X,int Y)
*  ����˵��������ֱ�߶�б��
*  �������أ���
*  �޸�ʱ�䣺2024��5��20��
*  ��    ע��
*
*************************************************************************/
float linearRegression(int path[][2], int start, int end) {
    int n = end - start + 1;
    float slope;
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = start; i <= end; ++i) {
        int x = path[i][0];
        int y = path[i][1];
        if(path[i][0]==4&&path[i-1][0]==4&&path[i+1][0]==4)return 0;
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    float denominator = (float)(n * sum_x2 - sum_x * sum_x);
    if (denominator == 0) {
        slope = 0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    return slope;
}
float linearRegression2zhong(int path[], int start, int end) {
    int n = end - start + 1;
    float slope;
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = start; i <= end; ++i) {
        int x = path[i];
        int y = i;  // ������Ϊ����ֵ
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    float denominator = (float)(n * sum_x2 - sum_x * sum_x);
    if (denominator == 0) {
        slope = 0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    return slope;
}
/*************************************************************************
*  �������ƣ�void leftxielv()
*  ����˵�����յ��⺯��
*  �������أ���
*  �޸�ʱ�䣺
*  ��    ע��
*
*************************************************************************/
void leftxielv(int leftstart,int rightstart)
{
    for(int i=0;i<Left_num-10;i++)
    {
        int x1,y1,x2,y2,x3,y3=0;
        x1=Image_Lefthand_Line[i][0];
        y1=Image_Lefthand_Line[i][1];
        x2=Image_Lefthand_Line[i+5][0];
        y2=Image_Lefthand_Line[i+5][1];
        x3=Image_Lefthand_Line[i+10][0];
        y3=Image_Lefthand_Line[i+10][1];
        if(y2<=20)break;
        if(x1<x2 && y1>y2 && x3<x2 && y3>y2)//�¹յ�
        {
            guaidian1[0][0]=x2;
            guaidian1[0][1]=y2;
        }
        if(x1<x2 && y1>y2 && x3<x2 && y3<y2)//�йյ�
        {
            guaidian1[1][0]=x2;
            guaidian1[1][1]=y2;
        }
        if(x1<x2 && y1<y2 && x3>x2 && y3<y2)//�Ϲյ�
        {
            guaidian1[2][0]=x2;
            guaidian1[2][1]=y2;
//            if(guaidian1[2][0]!=0&&guaidian1[2][0]<30)
//            {
//                guaidian1[2][0]=0;
//                guaidian1[2][1]=0;
//            }
        }
        if(x1<x2 && x3>x2 && abs(y1-y2)<5 && abs(y2-y3)>3)
        {
            guaidian1[3][0]=x2;//ʮ���Ϲյ�
            guaidian1[3][1]=y2;
        }
        if(x1<x2 && y2>y3 && abs(x2-x3)<5 && abs(y1-y2)<2 )
        {
            guaidian1[4][0]=x2;//�ϰ���ֱ�ǹյ�
            guaidian1[4][1]=y2;
        }
        if(x1<x2 && y1>y2 && x3>x2 && abs(y3-y2)<2 )
        {
            guaidian1[5][0]=x2;//�ϰ���۽ǹյ�
            guaidian1[5][1]=y2;
        }
        if(guaidian1[4][1]!=0&&guaidian1[4][1]>guaidian1[5][1])
        {
            guaidian1[4][0]=0;
            guaidian1[4][1]=0;
        }
        if(x1>x2 && abs(y1-y2)<3 && x3>x2 && y3<y2 )
        {
            guaidian1[6][0]=x2;//�ϰ�����ǹյ�
            guaidian1[6][1]=y2;
        }

        if(guaidian1[2][1]!=0&&guaidian1[2][1]>guaidian1[1][1])
        {
            guaidian1[1][0]=0;
            guaidian1[1][1]=0;
        }
        if(guaidian1[2][1]!=0&&guaidian1[2][1]>=guaidian1[0][1])
        {
            guaidian1[0][0]=0;
            guaidian1[0][1]=0;
        }
//        if(flag==1)
//        {
//            if(guaidian1[0][0]==0&&guaidian1[0][1]==0)
//            {
//                guaidian1[0][0]=4;
//                guaidian1[0][1]=115;
//            }
//        }
//        Image_Line[Image_Lefthand_Line[i][1]][Image_Lefthand_Line[i][0]]=1;
    }
    for(int i=0;i<Right_num-10;i++)
    {
        int X1,Y1,X2,Y2,X3,Y3=0;
        X1=Image_Righthand_Line[i][0];
        Y1=Image_Righthand_Line[i][1];
        X2=Image_Righthand_Line[i+5][0];
        Y2=Image_Righthand_Line[i+5][1];
        X3=Image_Righthand_Line[i+10][0];
        Y3=Image_Righthand_Line[i+10][1];
        if(Y2<=20)break;
        if(X1>X2 && Y1>Y2 && X3>X2 && Y3<Y2)
        {
            guaidian2[0][0]=X2;
            guaidian2[0][1]=Y2;
        }
        if(X1>X2 && Y1>Y2 && X3>X2 && Y3<Y2)
        {
            guaidian2[1][0]=X2;
            guaidian2[1][1]=Y2;
        }
        if(X1>X2 && Y1<Y2 && X3<X2 && Y3<Y2)
        {
            guaidian2[2][0]=X2;
            guaidian2[2][1]=Y2;
//            if(guaidian2[2][0]!=0&&159-guaidian2[2][0]<20)
//            {
//                guaidian2[2][0]=0;
//                guaidian2[2][1]=0;
//            }
        }
        if(X1>X2 && abs(Y1-Y2)<5 && X3<X2 && abs(Y2-Y3)>3)
        {
            guaidian2[3][0]=X2;//ʮ���Ϲյ�
            guaidian2[3][1]=Y2;
        }
        if(X1>X2 && Y2>Y3 && abs(X2-X3)<5 && abs(Y1-Y2)<2 )
        {
            guaidian2[4][0]=X2;//�ϰ���ֱ�ǹյ�
            guaidian2[4][1]=Y2;
        }
        if(X1>X2 && Y1>Y2 && X3<X2 && abs(Y3-Y2)<2 )
        {
            guaidian2[5][0]=X2;//�ϰ���۽ǹյ�
            guaidian2[5][1]=Y2;
        }
        if(guaidian2[4][1]!=0&&guaidian2[4][1]>guaidian2[5][1])
        {
            guaidian2[4][0]=0;
            guaidian2[4][1]=0;
        }
        if(X1<X2 && abs(Y1-Y2)<3 && X3<X2 && Y3<Y2 )
        {
            guaidian2[6][0]=X2;//�ϰ�����ǹյ�
            guaidian2[6][1]=Y2;
        }


        if(guaidian2[2][1]!=0&&guaidian2[2][1]>guaidian2[1][1])
        {
            guaidian2[1][0]=0;
            guaidian2[1][1]=0;
        }
        if(guaidian2[2][1]!=0&&guaidian2[2][1]>=guaidian2[0][1])
        {
            guaidian2[0][0]=0;
            guaidian2[0][1]=0;
        }
//        Image_Line[Image_Righthand_Line[i][1]][Image_Righthand_Line[i][0]]=1;
    }
}
/*************************************************************************
*  �������ƣ�float calculateCurvature(int path[][2], int idx, int path_length)();
*  ����˵����
*  ����˵�������ʼ��㺯��
*  �������أ�
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
float calculateCurvature(int path[][2], int idx, int path_length) {
    // ȷ�� idx �Ϸ�����������Խ��
    if (idx < 12 || idx + 12 >= path_length) {
        return 9999.0f; // ���� 0 ��ʾ�޷���������
    }

    // ��ȡ�����������
    int X1 = path[idx - 12][0];
    int Y1 = path[idx - 12][1];
    int X2 = path[idx][0];
    int Y2 = path[idx][1];
    int X3 = path[idx + 12][0];
    int Y3 = path[idx + 12][1];

    // ����������ABC�����
    float S_of_ABC = ((float)(X2 - X1) * (Y3 - Y1) - (float)(X3 - X1) * (Y2 - Y1)) / 2.0f;
    // ����ķ��ű�ʾ����

    // �������߳��ȵ�ƽ��
    float AB = sqrtf((float)((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)));
    float BC = sqrtf((float)((X3 - X2) * (X3 - X2) + (Y3 - Y2) * (Y3 - Y2)));
    float AC = sqrtf((float)((X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1)));

    // �ж��Ƿ��б߳�Ϊ0�����
    if (AB == 0.0f || BC == 0.0f || AC == 0.0f) {
        return -0.1f;
    }

    // ��������
    float K = (4.0f * fabsf(S_of_ABC)) / (AB * BC * AC);

    return K;
}
//
//float calculateCurvature(int path[][2], int idx) {
//    int X1 = path[idx - 15][0];
//    int Y1 = path[idx - 15][1];
//    int X2 = path[idx][0];
//    int Y2 = path[idx][1];
//    int X3 = path[idx + 15][0];
//    int Y3 = path[idx + 15][1];
//    float K;
//    int S_of_ABC = ((X2 - X1) * (Y3 - Y1) - (X3 - X1) * (Y2 - Y1)) / 2;
//    //����ķ��ű�ʾ����
//    float q1 = (float)((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
//    float AB = sqrtf(q1);
//    q1 = (float)((X3 - X2) * (X3 - X2) + (Y3 - Y2) * (Y3 - Y2));
//    float BC = sqrtf(q1);
//    q1 = (float)((X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1));
//    float AC =  sqrtf(q1);
//    if (AB * BC * AC == 0)
//      {
//          K = 0;
//      }
//    else
//       K = (float)4  * S_of_ABC / (AB * BC * AC);
//   return K;
//
////    double denominator1 = (double)(x1 - x2);
////    double denominator2 = (double)(x2 - x3);
////
////    // ����ĸ�Ƿ�Ϊ�㣬��ֹ�������
////    if (denominator1 == 0 || denominator2 == 0) {
////        return -0.1; // ��������������ʷǳ�С��������Ϊ��ֱ��
////    }
////
////    double k1 = 0.5 * (x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2) / denominator1;
////    double k2 = 0.5 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) / denominator2;
////
////    double centerX = (k1 - k2) / (y1 - y2 - y2 + y3);
////    double centerY = k1 - centerX * (y1 - y2) / denominator1;
////
////    double radius = sqrt((x1 - centerX) * (x1 - centerX) + (y1 - centerY) * (y1 - centerY));
////
////    // ���뾶�Ƿ�Ϊ�㣬��ֹ�������
////    if (radius == 0) {
////        return 9999; // ��������������ʷǳ��󣬿�����Ϊ�ǹյ�
////    }
////
////    return 1 / radius;
//}
/*************************************************************************
*  �������ƣ�void blur();
*  ����˵����
*  ����˵�������ߵ㼯�����˲�
*  �������أ�
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
void blur(int ptsin[][2],int *num,int ptsout[][2],int he)
{
    if(he%2==1)
    {
       int half=he/2;
       for(int i=0;i<*num;i++)
       {
           ptsout[i][0]=ptsout[i][1]=0;
           for(int j= -half;j<=half;j++)
           {
               int index = i + j;
               if (index < 0) {
                   index = 0; // ������ֵ������ 0 �� num - 1 ֮��
               } else if (index >= *num) {
                   index = *num - 1;
               }
               ptsout[i][0] += ptsin[index][0] * (half + 1 - abs(j));
               ptsout[i][1] += ptsin[index][1] * (half + 1 - abs(j));
           }
           ptsout[i][0] /= (2 * half + 2) * (half + 1) / 2;
           ptsout[i][1] /= (2 * half + 2) * (half + 1) / 2;
       }
    }
}
/*************************************************************************
*  �������ƣ�void find();
*  ����˵����
*  ����˵�����жϵ�ǰ����״̬
*  �������أ�
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
int find(float currentslope)
{
    if(fabsf(currentslope)<1.9&&fabsf(currentslope)>=1.0)return 0;
    if(fabsf(currentslope)<1.0&&fabsf(currentslope)>0.2)return 1;
    if(fabsf(currentslope)<=0.2&&fabsf(currentslope)>0)return 2;
    if(fabsf(currentslope)==0)return 3;
    return 99;
}
/*************************************************************************
*  �������ƣ�void draw_line();
*  ����˵����
*  ����˵�������ߺ���������ֱ��
*  �������أ�
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
void draw_line(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        Image_Line[(int)y][(int)x] = 1;
        x += xIncrement;
        y += yIncrement;
    }
}
int Calc_ERROR(void)
{
    int Err=0;
    if(zhangaiflag==0)
    {
        for(int i=0;i<5;i++)
        {
            int error=70-Image_centralLine[Look_Line-i-Image_upLine];
            image_error[i]=error;
            Err+=error;
        }
        Err = Err/5;
        image_error[5]=Err;
    }
    if(zhangaiflag==1)
    {
        Err=70-Image_centralLine[tmperr+5-Image_upLine];
    }
    if(zhangaiflag==2)
    {
        Err=70-Image_centralLine[tmperr+5-Image_upLine];
    }
    //�������
    return Err;
}
/*************************************************************************
*  �������ƣ�void qulv();
*  ����˵����
*  ����˵�������ʼ���
*  �������أ�
*  �޸�ʱ�䣺
*  ��    ע��
*************************************************************************/
void qulv()
{
    qulv_value=0;
    qulv_jin=0;
    qulv_yuan=0;
    qvlv_quanju_right=0;
    qvlv_quanju_left=0;
    qvlv_jin_left=0;
    qvlv_jin_right=0;
    qvlv_yuan_left=0;
    qvlv_yuan_right=0;
    for(int i=central_length;i>0;i--)
    {
        if((Image_centralLine[i]-Image_centralLine[i-1])>0)
         {
           if(i>45)
           {
               qvlv_jin_right++;
           }
           if(i<50)
           {
               qvlv_yuan_right++;
           }
           qvlv_quanju_right++;
         }
        else if((Image_centralLine[i]-Image_centralLine[i-1])<0)
         {
            if(i>45)
            {
                qvlv_jin_left++;
            }
            if(i<50)
            {
                qvlv_yuan_left++;
            }
           qvlv_quanju_left++;
         }
    }
    qulv_value=qvlv_quanju_right-qvlv_quanju_left;
    qulv_jin=qvlv_jin_right-qvlv_jin_left;
    qulv_yuan=qvlv_yuan_right-qvlv_yuan_left;
//    if(qvlv_quanju_right>15&&qvlv_quanju_left>15&&qulv_value<10&&current_state1!=20&&current_state2!=3)
//    {
//        Beep_on;
//    }
}
