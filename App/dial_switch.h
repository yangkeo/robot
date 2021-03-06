//-------------------------------------------------------------------------*
// 文件名: dial_switch.h                                                           *
// 说  明: 8位拨码开关驱动函数头文件                                             *
//-------------------------------------------------------------------------*

#ifndef _dial_switch_H_
#define _dial_switch_H_

#include "common.h"
#include "MK60_gpio.h"

/*==========================拨码开关使用的引脚=============================*/
#define switch1        PTB17            
#define switch2        PTB16        
#define switch3        PTB11        
#define switch4        PTB10      
#define switch5        PTB9       
#define switch6        PTB8       
#define switch7        PTB7       
#define switch8        PTB6     

extern uint8 switch_state;       //拨码开关状态量
extern uint8 switch_1_4;         //1~4位拨码开关状态量

//-------------------------------------------------------------------------*
//函数名: Read_switch                                                     
//功  能: 读取拨码开关值                                                  
//说  明：通过全局变量出口开关数值
//
//-------------------------------------------------------------------------*
void Read_switch(void);

//-------------------------------------------------------------------------*
//函数名: switch_init_all                                                     *
//功  能: 初始化所有拨码开关状态                                             *
//-------------------------------------------------------------------------*
void switch_init_all(void);

#endif