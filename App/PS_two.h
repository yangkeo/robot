#ifndef __PS_two_H__
#define __PS_two_H__;
//#include "delay.h"
//#include "sys.h"

#define PS_DI PTE6
#define PS_DO PTE7
#define PS_CS PTE8
#define PS_CLK PTE9



#define DI   PS_DATA_IN()           //PB12  ����

#define DO_H PS_DATA_DO(1)        //����λ��
#define DO_L PS_DATA_DO(0)        //����λ��

#define CS_H PS_DATA_CS(1)        //cs����
#define CS_L PS_DATA_CS(0)        //cs����

#define CLK_H PS_DATA_CLK(1)      //ʱ������
#define CLK_L PS_DATA_CLK(0)      //ʱ������


//These are our button constants
#define PSB_SELECT      1
#define PSB_L3          2
#define PSB_R3          3
#define PSB_START       4
#define PSB_PAD_UP      5
#define PSB_PAD_RIGHT   6
#define PSB_PAD_DOWN    7
#define PSB_PAD_LEFT    8
#define PSB_L2          9
#define PSB_R2          10
#define PSB_L1          11
#define PSB_R1          12
#define PSB_GREEN       13
#define PSB_RED         14
#define PSB_BLUE        15
#define PSB_PINK        16
#define PSB_TRIANGLE    13
#define PSB_CIRCLE      14
#define PSB_CROSS       15
#define PSB_SQUARE      26

//#define WHAMMY_BAR		8

//These are stick values
#define PSS_RX 5                //��ҡ��X������
#define PSS_RY 6
#define PSS_LX 7
#define PSS_LY 8



extern unsigned char Data[9];
extern unsigned int MASK[16];
extern unsigned int Handkey;

void PS2_Init(void);
unsigned char PS2_RedLight(void);  //�ж��Ƿ�Ϊ���ģʽ
void PS2_ReadData(void);            //��ң��������������
void PS2_Cmd(unsigned char CMD);   //��ң����������
unsigned char PS2_DataKey(void);		        //�Զ�������ps2�����ݽ��д���     ֻ�����˰�������
unsigned char PS2_AnologData(unsigned char button);  //�õ�һ��ҡ�˵�ģ����
void PS2_ClearData(void);	  //������ݻ�����

/********************************  ��д���� ****************************************/

void delay_PS(void);
unsigned char PS_DATA_IN(void);
void PS_DATA_DO(unsigned char c);
void PS_DATA_CS(unsigned char c);
void PS_DATA_CLK(unsigned char c);

void Mechanical_arm(void);
//void Mechanical_arm_2(void);

#endif
