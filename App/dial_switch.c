//-------------------------------------------------------------------------*
// �ļ���: dial_switch.c                                                        *
// ˵  ��: 8λ���뿪�����������ļ�                                         *
//-------------------------------------------------------------------------*
#include "common.h"
#include "include.h"
#include "MK60_gpio.h"
#include "dial_switch.h"

uint8 switch_state;       //8λ���뿪��״̬��
uint8 switch_1_8;         //1~8λ���뿪��״̬��                                             *
//-------------------------------------------------------------------------*
void switch_init_all(void)
{
	gpio_init(switch1,GPI,1);//��ʼ�����뿪��1
	gpio_init(switch2,GPI,1);//��ʼ�����뿪��2
	gpio_init(switch3,GPI,1);//��ʼ�����뿪��3
	gpio_init(switch4,GPI,1);//��ʼ�����뿪��4
	gpio_init(switch5,GPI,1);//��ʼ�����뿪��5
	gpio_init(switch6,GPI,1);//��ʼ�����뿪��6
	gpio_init(switch7,GPI,1);//��ʼ�����뿪��7
	gpio_init(switch8,GPI,1);//��ʼ�����뿪��8
	Read_switch();
}
//-------------------------------------------------------------------------*
//������: Read_switch                                                     
//��  ��: ��ȡ���뿪��ֵ                                                  
//˵  ����ͨ��ȫ�ֱ������ڿ�����ֵ
//
//-------------------------------------------------------------------------*
void Read_switch(void)
{
	switch_state = 0x00;
	switch_state |= gpio_get(switch1);
	switch_state |= gpio_get(switch2)<<1;
	switch_state |= gpio_get(switch3)<<2;
	switch_state |= gpio_get(switch4)<<3;	
	switch_state |= gpio_get(switch5)<<4;
	switch_state |= gpio_get(switch6)<<5;
	switch_state |= gpio_get(switch7)<<6;
	switch_state |= gpio_get(switch8)<<7;
}