#include "common.h"
#include "include.h"
//�������˿�����
//========================================================
#define BUZZER_ON()               gpio_set (PTE10, 0); //��gpio_ctrl(PORTE,10,0);
#define BUZZER_OFF()              gpio_set (PTE10, 1); //���� 
/////////////////////////////////////////////////////////////////
void buzzer_on_off(uint8 num);   //����������ȡ��
void buzzer_main(void);     //�������жϿ��ƺ���
void buzzer_test(void);     //���������Ժ�������һ��
void buzzer_init(void);    //��������ʼ������
void buzzer_config(void);   //���������ú���
//////////////////////////////////////////////////////////////// 
uint8 buzzer_count;
/*========================================================================================================================
                             �����                                                                                        
=========================================================================================================================*/
void buzzer_config(void) 
{
	gpio_init (PTE10,GPO,1); 
}
void buzzer_init(void) 
{
	buzzer_config();
	BUZZER_OFF(); 
	buzzer_count=0;
}
/*========================================================================================================================
                             Ӧ�ò�                                                                                      
=========================================================================================================================*/
///////////////////////////////////////////////////////////////////////////////
void buzzer_Delay(void)  //1ms 
{
	unsigned int xx,xxx;
	for(xx=0;xx<1;xx++)                       
	for(xxx=0;xxx<36000;xxx++);
}
//���������Ժ�������һ��
void buzzer_test(void) 
{
	uint8 i;
	for(i=0;i<100;i++)
	{
		BUZZER_ON();
		buzzer_Delay(); 
		BUZZER_OFF();
		buzzer_Delay();
	}
}
/*//����������ȡ��
void buzzer_on_off(uint8 num) 
{
    buzzer_count++;
	if(buzzer_count>=num)
	{
		gpio_reverse(PORTD,15);
		buzzer_count=0;
	}
}*/
void buzzer_main(void) 
{

	
}
  
  
  
  
  
  
  