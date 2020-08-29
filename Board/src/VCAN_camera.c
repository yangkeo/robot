#include "common.h"


//压缩二值化图像解压（空间 换 时间 解压）
//srclen 是二值化图像的占用空间大小
//【鹰眼解压】鹰眼图像解压，转为 二维数组 - 智能车资料区 - 山外论坛 http://vcan123.com/forum.php?mod=viewthread&tid=17&ctid=6
//解压的时候，里面有个数组，配置黑、白对应的值是多少。
void img_extract(void *dst, void *src, uint32_t srclen)
{
    uint8_t colour[2] = {255, 0}; //0 和 1 分别对应的颜色
    uint8_t * mdst = dst;
    uint8_t * msrc = src;
    //注：山外的摄像头 0 表示 白色，1表示 黑色
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
//函数名：  void img_condense
//输入：     void *dst
//输出：     void *src
//说明：  将二维数组压缩成一维数组
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
///////////////将二维数组数值为 （b）1111 1111 转换为 （b） 0000 0001////////////
    while(srclen--)                                                             
    {    
         tmpsrc1 = *(msrc1++);                                               //（b）0000 0000 转换为 （b） 0000 0000
         if(tmpsrc1==255)
         {
           
              *msrc1--;                                                      //指针之所以先加 1 又减 1 是为了避免系统优化
              *msrc1= 0;                                                     //原来没有先加 1 系统自动优化，while只执行一次
              msrc1++;
              
         }
         else
         {
              *msrc1--;
              *msrc1= 1;
              msrc1++;
         
         }
    }
 
//////////////每二维数组里八个数合并成一维数组里一个数，//////////////////
//////////////每个数对应新数的位，共八位，从高到低依次放到新数里////////   
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

