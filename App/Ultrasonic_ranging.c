#include "common.h"
#include "include.h"

#define Trig  PTE4
#define Echo  PTE5

#define flag_wait  0XFFFFF

uint32 timevar;

void Ultrasonic_init()
{
       gpio_init(Trig,GPO,0);
       gpio_init(Echo,GPI,0);
}
void Ultrasonic_ranging()
{
        uint32 flag;
        flag = 0;
        gpio_set(Trig,1);               //������������
        pit_delay_us(PIT1,15);
        gpio_set(Trig,0);
      
        while(gpio_get(Echo) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        
        pit_time_start  (PIT0);                 //��ʼ��ʱ
        
        while(gpio_get(Echo) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag >flag_wait)
            {
                break;
            }
        };             
        
        timevar = pit_time_get_us(PIT0);    //ֹͣ��ʱ����ȡ��ʱʱ��
        if(flag <flag_wait)
        {
            timevar = timevar * 340 /2/1000;
                
            if(timevar > 5)
            {
                printf("\n\n����Ϊ��%dmm",timevar); //��ӡ��ʱʱ��
            }
        }
        VCAN_Send(timevar,2,3,4,5,6,7,8);
        //Delay_ms(10); 
        //DELAY_MS(1000); 
    }
