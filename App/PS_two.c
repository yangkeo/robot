#include "PS_two.h"
#include "include.h"

float a_1 = 0;
float a_2 = 0;
float a_3 = 0;
float k_1 = 1.1;
float b_1 = 129;
float k_2 = 1.09;
float b_2 = 140;
float k_3 = 0.74;
float b_3 = 180;
unsigned int angle_1 = 0;
unsigned int angle_2 = 0;
unsigned int angle_3 = 0;
unsigned int out_d = 270;

unsigned int Handkey;
unsigned char Comd[2]={0x01,0x42};//��ʼ����
unsigned char Data[9]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; //���ݴ洢����
unsigned int MASK[]={
    PSB_SELECT,
    PSB_L3,
    PSB_R3 ,
    PSB_START,
    PSB_PAD_UP,
    PSB_PAD_RIGHT,
    PSB_PAD_DOWN,
    PSB_PAD_LEFT,
    PSB_L2,
    PSB_R2,
    PSB_L1,
    PSB_R1 ,
    PSB_GREEN,
    PSB_RED,
    PSB_BLUE,
    PSB_PINK
	};	//����ֵ�����




void PS2_Init(void)   //  �ֱ��������˿ڽ��� ���������������ģ�
{
  gpio_init (PS_DI, GPI,0);       //��ʼ�� PTE7 �ܽ�Ϊ����
  gpio_init (PS_DO, GPO,0);       //��ʼ�� PTE6 �ܽ�Ϊ���
  gpio_init (PS_CS, GPO,0);       //PTE8 ���   
  gpio_init (PS_CLK, GPO,0);       //PTE9 ���
}

void Mechanical_arm(void)
{
  int i = 0;
  //PS2_ClearData();   //�������
  //PS2_ReadData();    //������
  a_1 = (( PS2_AnologData(PSS_RX) )*k_1+b_1);         
  angle_1 = (unsigned int)a_1;
  setPWM(0x0c,0,angle_1);
          
  a_2 = (( PS2_AnologData(PSS_RY) )*k_2+b_2);
  angle_2 = (unsigned int)a_2;
  setPWM(0x0d,0,angle_2);
  setPWM(0x0e,0,angle_2);
           
  a_3 = (( PS2_AnologData(PSS_LY) )*k_3+b_3);
  angle_3 = (unsigned int)a_3;
  if(angle_3 > 300)
  {
     angle_3 = 300;
  }
  setPWM(0x0f,0,angle_3);
  
  
  if(Data[3] == 0xfd)
  {
    setPWM(0x0f,0,180);
  }
  if(Data[3] == 0xfb)
  {
    setPWM(0x0f,0,290);
  }
  
  for(i = 0; i < 5; i++)
  {
    delay_PS();
  }
           
 
}



/*void Mechanical_arm(void)
{
  if(Data[3] == 0xfd)
  {
    setPWM(0x0f,0,180);
  }
  if(Data[3] == 0xfb)
  {
    setPWM(0x0f,0,290);
  }
}*/

//��ң������������
void PS2_Cmd(unsigned char CMD)
{
  volatile unsigned int ref=0x01;
  Data[1] = 0;
  for(ref=0x01;ref<0x0100;ref<<=1)
  {
    if(ref&CMD)
    {
       DO_H;                   //����ߵ�ƽ
    }
    else DO_L;

    CLK_H;                    //ʱ������
    delay_PS();
    CLK_L;
    delay_PS();
    CLK_H;
    if(DI)
     Data[1] = ref|Data[1];
   }
}


//�ж��Ƿ�ΪΪ���ģʽ
unsigned char PS2_RedLight(void)
{
  CS_L;
  PS2_Cmd(Comd[0]);  //��ʼ����
  PS2_Cmd(Comd[1]);  //��������
  CS_H;
  if( Data[1] == 0X73)   
  {
    return 0 ;       //  ���ģʽ    
  }
  else 
  {
    return 1;        //  �̵�ģʽ
  }

}


//��ȡң��������
void PS2_ReadData(void)
{
  volatile unsigned char byte=0;
  volatile unsigned int ref=0x01;

  CS_L;

  PS2_Cmd(Comd[0]);  //��ʼ����
  PS2_Cmd(Comd[1]);  //��������

  for(byte=2;byte<9;byte++)          //��ʼ��������
  {
    for(ref=0x01;ref<0x100;ref<<=1)
    {
      CLK_H;
      CLK_L;
      delay_PS();
      CLK_H;
      if(DI)
      {
        Data[byte] = ref|Data[byte];
      }      		      
    }
  delay_PS();
   }
  CS_H;	
}

//�Զ�������ps2�����ݽ��д���     ֻ�����˰�������    Ĭ�������Ǻ��ģʽ    ֻ��һ����������ʱ
//����Ϊ0�� δ����Ϊ1
unsigned char PS2_DataKey(void)
{
  unsigned char index;

  PS2_ClearData();
  PS2_ReadData();

  Handkey=(Data[4]<<8)|Data[3];     //����16������  ����Ϊ0��  δ����Ϊ1
  for(index=0;index<16;index++)
  {	    
     if((Handkey&(1<<(MASK[index]-1)))==0)
     return index+1;
  }
   return 0;          //û���κΰ�������
}

//�õ�һ��ҡ�˵�ģ����  ��Χ0~256
unsigned char PS2_AnologData(unsigned char button)
{
  int x = 0;
  x = Data[button];
  return Data[button];
}

//������ݻ�����
void PS2_ClearData()
{
  unsigned char i;
  for(i=0;i<9;i++)
  {
     Data[i]=0x00;
  }     
}



/********************************  ��д���� ************************************/
void delay_PS(void)
{
  int i;
  for(i=300;i>0;i--);
} 


unsigned char PS_DATA_IN(void)       //��ң��������
{
  unsigned char c = 0;
  c = gpio_get (PS_DI);
  return(c);
}

void PS_DATA_DO(unsigned char c)     //��ң����������
{
  if(c==0x00)
  {
    gpio_set (PS_DO, 0);                  
  }
  else
  {
    gpio_set (PS_DO, 1);                  
  }
}

void PS_DATA_CS(unsigned char c)    //ң���������ź����
{ 
  if(c==0x00)
  {
    gpio_set (PS_CS, 0);                  
  }
  else
  {
    gpio_set (PS_CS, 1);                    
  }
}

void PS_DATA_CLK(unsigned char c)    //ʱ���ź����
{
  if(c==0x00)
  {
    gpio_set (PS_CLK, 0);                  
  }
  else
  {
    gpio_set (PS_CLK, 1);                    
  }
}

/*******************************************************************************/
