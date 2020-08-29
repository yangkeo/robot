#include "common.h"
#include "include.h"
#include "time.h"
#include "dial_switch.h"

  
void PIT0_IRQHandler(void);                    //��ʱ���жϷ�����򣺶�������������Զ��
void PIT1_IRQHandler(void);
void time_init()
{
     pit_init_ms(PIT0,50);                                    //��ʼ��PIT0����ʱʱ��Ϊ�� 3ms
     set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);       //����PIT0���жϷ�����Ϊ PIT0_IRQHandler
     disable_irq(PIT0_IRQn);
}
/*! 
 *  @brief      PIT0�жϷ�����
 *  @since      v5.0
 */
void PIT0_IRQHandler(void)
{
   
     
     PIT_Flag_Clear(PIT0);                        //���жϱ�־λ
   
}




