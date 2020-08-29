#include "common.h"


//ѹ����ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
//srclen �Ƕ�ֵ��ͼ���ռ�ÿռ��С
//��ӥ�۽�ѹ��ӥ��ͼ���ѹ��תΪ ��ά���� - ���ܳ������� - ɽ����̳ http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//��ѹ��ʱ�������и����飬���úڡ��׶�Ӧ��ֵ�Ƕ��١�
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 �� 1 �ֱ��Ӧ����ɫ
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8_t tmpsrc1;
    while(srclen --)
    {
        tmpsrc1 = *msrc++;
        *mdst++ = colour[ (tmpsrc1 >> 7 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 6 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 5 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 4 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 3 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 2 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 1 ) & 0x01 ];
        *mdst++ = colour[ (tmpsrc1 >> 0 ) & 0x01 ];
    }
} 

/*=============================================================================*/
//��������  void img_condense
//���룺     void *dst
//�����     void *src
//˵����  ����ά����ѹ����һά����
/*==============================================================================*/

 void img_condense( void *dst, void *src, uint32_t srclen)
 {   
    uint8_t * mdst = dst;
    uint8_t * msrc1 = src;
    uint8_t * msrc2 = src;
    uint8_t tmpsrc1;
    uint8_t tmpsrc2 =0;
    uint32_t srclen2;
    srclen2 = srclen/8;
///////////////����ά������ֵΪ ��b��1111 1111 ת��Ϊ ��b�� 0000 0001////////////
    while(srclen--)                                                             
    {    
         tmpsrc1 = *(msrc1++);                                               //��b��0000 0000 ת��Ϊ ��b�� 0000 0000
         if(tmpsrc1==255)
         {
           
              *msrc1--;                                                      //ָ��֮�����ȼ� 1 �ּ� 1 ��Ϊ�˱���ϵͳ�Ż�
              *msrc1= 0;                                                     //ԭ��û���ȼ� 1 ϵͳ�Զ��Ż���whileִֻ��һ��
              msrc1++;
              
         }
         else
         {
              *msrc1--;
              *msrc1= 1;
              msrc1++;
         
         }
    }
 
//////////////ÿ��ά������˸����ϲ���һά������һ������//////////////////
//////////////ÿ������Ӧ������λ������λ���Ӹߵ������ηŵ�������////////   
    while(srclen2--)                                                         
    {
        uint8_t i;      
        for(i=0; i<8; i++)
        {   
            tmpsrc1 = *msrc2;
            tmpsrc1 = (tmpsrc1 << (7-i) );
            *mdst = tmpsrc1 | tmpsrc2 ;
            tmpsrc2 = *mdst;
            msrc2++;
        }
        tmpsrc2=0;
        mdst++;       
    }
   
 }

