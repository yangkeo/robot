/*****************************************************************************************************************************/

#include "common.h"
#include "include.h"
#include "MK60_gpio.h"
#include "MK60_port.h"
#include "MK60_dma.h"
#include "VCAN_camera.h"
#include "MK60_lptmr.h"

//#define OV7725_EAGLE_Delay_ms(time)  OV7725_EAGLE_Delay_ms(time)


uint8   *ov7725_eagle_img_buff;

volatile IMG_STATUS_e      ov7725_eagle_img_flag = IMG_FINISH;   //图像状态

//内部函数声明
static uint8 ov7725_eagle_reg_init(void);
static void ov7725_eagle_port_init();
void PORTB_IRQHandler();
void DMA0_IRQHandler();

/*****************************************************************************************************************************/
/*!
 *  @brief      鹰眼ov7725初始化
 *  @since      v5.0
 */
uint8 ov7725_eagle_init(uint8 *imgaddr)
{
    ov7725_eagle_img_buff = imgaddr;
    while(ov7725_eagle_reg_init() == 0);
    ov7725_eagle_port_init();
    set_vector_handler(PORTB_VECTORn , PORTB_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
    return 0;
}

/*!
 *  @brief      鹰眼ov7725管脚初始化（内部调用）
 *  @since      v5.0
 */
void ov7725_eagle_port_init()
{
    //DMA通道0初始化，PTA27触发源(默认上升沿)，源地址为PTB_B0_IN，目的地址为：IMG_BUFF，每次传输1Byte
    dma_portx2buff_init(OV7725_EAGLE_DMA_CH, (void *)&PTC_B1_IN, (void *)ov7725_eagle_img_buff, PTA8, DMA_BYTE1, OV7725_EAGLE_DMA_NUM, DADDR_KEEPON);

    DMA_DIS(OV7725_EAGLE_DMA_CH);
    disable_irq(PORTB_IRQn);                        //关闭PTA的中断
    DMA_IRQ_CLEAN(OV7725_EAGLE_DMA_CH);                   //清除通道传输中断标志位
    DMA_IRQ_EN(OV7725_EAGLE_DMA_CH);
    port_init(PTA8, ALT1 | DMA_FALLING | PULLDOWN );         //PCLK

    port_init(PTB21, ALT1 | IRQ_RISING  | PULLDOWN | PF);     //场中断，上拉，上降沿触发中断，带滤波

}

/*!
 *  @brief      鹰眼ov7725场中断服务函数
 *  @since      v5.0
 */
void ov7725_eagle_vsync(void)
{

    //场中断需要判断是场结束还是场开始
    if(ov7725_eagle_img_flag == IMG_START)                   //需要开始采集图像
    {
        ov7725_eagle_img_flag = IMG_GATHER;                  //标记图像采集中
        disable_irq(PORTB_IRQn);

#if 1

        PORTA_ISFR = 1 <<  8;            //清空PCLK标志位

        DMA_EN(OV7725_EAGLE_DMA_CH);                  //使能通道CHn 硬件请求
        PORTA_ISFR = 1 <<  8;            //清空PCLK标志位
        DMA_DADDR(OV7725_EAGLE_DMA_CH) = (uint32)ov7725_eagle_img_buff;    //恢复地址

#else
        PORTA_ISFR = 1 <<  8;            //清空PCLK标志位
        dma_repeat(OV7725_EAGLE_DMA_CH, (void *)&PTC_B1_IN, (void *)ov7725_eagle_img_buff,OV7725_EAGLE_DMA_NUM);
#endif
    }
    else                                        //图像采集错误
    {
        disable_irq(PORTB_IRQn);                        //关闭PTA的中断
        ov7725_eagle_img_flag = IMG_FAIL;                    //标记图像采集失败
    }
}

/*!
 *  @brief      鹰眼ov7725 DMA中断服务函数
 *  @since      v5.0
 */
void ov7725_eagle_dma()
{
    ov7725_eagle_img_flag = IMG_FINISH ;
    DMA_IRQ_CLEAN(OV7725_EAGLE_DMA_CH);           //清除通道传输中断标志位
}

/*!
 *  @brief      鹰眼ov7725采集图像（采集到的数据存储在 初始化时配置的地址上）
 *  @since      v5.0
 */
void ov7725_eagle_get_img()
{
    ov7725_eagle_img_flag = IMG_START;                   //开始采集图像
    PORTB_ISFR = ~0;                        //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    enable_irq(PORTB_IRQn);                         //允许PTA的中断
    while(ov7725_eagle_img_flag != IMG_FINISH)           //等待图像采集完毕
    {
        if(ov7725_eagle_img_flag == IMG_FAIL)            //假如图像采集错误，则重新开始采集
        {
            ov7725_eagle_img_flag = IMG_START;           //开始采集图像
            PORTB_ISFR = ~0;                        //写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
            enable_irq(PORTB_IRQn);                 //允许PTA的中断
        }
    }
}
/*OV7725初始化配置表*/
reg_s ov7725_eagle_reg[] =
{
    //寄存器，寄存器值次
    {OV7725_RED          , 0x80},
    {OV7725_COM4         , 0x41},//0x41和0x00表示75帧；0xC1和0x02  50帧；0xC1和0x00  150...
    {OV7725_CLKRC        , 0x00},
    {OV7725_COM2         , 0x03},
    {OV7725_COM3         , 0xD0},
    {OV7725_COM7         , 0x40},
    {OV7725_COM10        , 0x20},
    {OV7725_HSTART       , 0x3F},
    {OV7725_HSIZE        , 0x50},
    {OV7725_VSTRT        , 0x03},
    {OV7725_VSIZE        , 0x78},
    {OV7725_HREF         , 0x00},
    {OV7725_SCAL0        , 0x0A},
    {OV7725_AWB_Ctrl0    , 0xE0},
    {OV7725_DSPAuto      , 0xff},
    {OV7725_DSP_Ctrl2    , 0x0C},
    {OV7725_DSP_Ctrl3    , 0x00},
    {OV7725_DSP_Ctrl4    , 0x00},

#if (OV7725_EAGLE_W == 80)
    {OV7725_HOutSize     , 0x14},
#elif (OV7725_EAGLE_W == 160)
    {OV7725_HOutSize     , 0x28},
#elif (OV7725_EAGLE_W == 240)
    {OV7725_HOutSize     , 0x3c},
#elif (OV7725_EAGLE_W == 320)
    {OV7725_HOutSize     , 0x50},
#else

#endif

#if (CAMERA_H == 60 )
    {OV7725_VOutSize     , 0x1E},
#elif (CAMERA_H == 120 )
    {OV7725_VOutSize     , 0x3c},
#elif (CAMERA_H == 180 )
    {OV7725_VOutSize     , 0x5a},
#elif (CAMERA_H == 240 )
    {OV7725_VOutSize     , 0x78},
#else

#endif

    {OV7725_EXHCH        , 0x00},
    {OV7725_GAM1         , 0x0c},
    {OV7725_GAM2         , 0x16},
    {OV7725_GAM3         , 0x2a},
    {OV7725_GAM4         , 0x4e},
    {OV7725_GAM5         , 0x61},
    {OV7725_GAM6         , 0x6f},
    {OV7725_GAM7         , 0x7b},
    {OV7725_GAM8         , 0x86},
    {OV7725_GAM9         , 0x8e},
    {OV7725_GAM10        , 0x97},
    {OV7725_GAM11        , 0xa4},
    {OV7725_GAM12        , 0xaf},
    {OV7725_GAM13        , 0xc5},
    {OV7725_GAM14        , 0xd7},
    {OV7725_GAM15        , 0xe8},
    {OV7725_SLOP         , 0x20},
    {OV7725_LC_RADI      , 0x00},
    {OV7725_LC_COEF      , 0x13},
    {OV7725_LC_XC        , 0x08},
    {OV7725_LC_COEFB     , 0x14},
    {OV7725_LC_COEFR     , 0x17},
    {OV7725_LC_CTR       , 0x05},
    {OV7725_BDBase       , 0x99},
    {OV7725_BDMStep      , 0x03},
    {OV7725_SDE          , 0x04},
    {OV7725_BRIGHT       , 0x00},
    {OV7725_CNST         , 0x45},                //00时全黑  ff最大  王艺飞
    {OV7725_SIGN         , 0x06},
    {OV7725_UVADJ0       , 0x11},
    {OV7725_UVADJ1       , 0x02},

};

uint8 ov7725_eagle_cfgnum = ARR_SIZE( ov7725_eagle_reg ) ; /*结构体数组成员数目*/

/*****************************************************************************************************************************/
/*!
 *  @brief      鹰眼ov7725寄存器 初始化
 *  @return     初始化结果（0表示失败，1表示成功）
 *  @since      v5.0
 */
uint8 ov7725_eagle_reg_init(void)
{
    uint16 i = 0;
    uint8 Sensor_IDCode = 0;
    SCCB_GPIO_init();

    //OV7725_Delay_ms(50);
    if( 0 == SCCB_WriteByte ( OV7725_COM7, 0x80 ) ) /*复位sensor */
    {
        DEBUG_PRINTF("\n警告:SCCB写数据错误");
        return 0 ;
    }

    lptmr_delay_ms(50); 

    if( 0 == SCCB_ReadByte( &Sensor_IDCode, 1, OV7725_VER ) )    /* 读取sensor ID号*/
    {
        DEBUG_PRINTF("\n警告:读取ID失败");
        return 0;
    }
    DEBUG_PRINTF("\nGet ID success，SENSOR ID is 0x%x", Sensor_IDCode);
    DEBUG_PRINTF("\nConfig Register Number is %d ", ov7725_eagle_cfgnum);
    if(Sensor_IDCode == OV7725_ID)
        {
        for( i = 0 ; i < ov7725_eagle_cfgnum ; i++ )
        {
            if( 0 == SCCB_WriteByte(ov7725_eagle_reg[i].addr, ov7725_eagle_reg[i].val) )
            {
                DEBUG_PRINTF("\n警告:写寄存器0x%x失败", ov7725_eagle_reg[i].addr);
                return 0;
            }
        }
    }
    else
    {
        return 0;
    }
    DEBUG_PRINTF("\nOV7725 Register Config Success!");
    return 1;
}
/*!
 *  @brief      PORTA中断服务函数
 *  @since      v5.0
 */
void PORTB_IRQHandler()
{
    uint32 flag;

    while(!PORTB_ISFR);
    flag = PORTB_ISFR;
    PORTB_ISFR  = ~0;                                   //清中断标志位
    flag = (flag << 8);
    if(flag & (0x20000000))                                 //PTA29触发中断
    {
        ov7725_eagle_vsync();
    }
    
    /*              现在没使用行中断控制触发
#if ( CAMERA_USE_HREF == 1 )                            //使用行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif*/

}

/*!
 *  @brief      DMA0中断服务函数
 *  @since      v5.0
 */
void DMA0_IRQHandler()
{
    ov7725_eagle_dma();
}





