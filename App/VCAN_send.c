#include "include.h"

/////////////////////////////////////////////////////////////////////////////////////
//���ݲ�ֺ궨�壬�ڷ��ʹ���1�ֽڵ���������ʱ������int16��float�ȣ���Ҫ�����ݲ�ֳɵ����ֽڽ��з���
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp)	  ) )
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

uint8 vcan_to_send[20];	//�������ݻ���


void VCAN_Send(uint16 a,uint16 b,uint16 c,uint16 d,uint16 e,uint16 f,uint16 g,uint16 h)   //ͷ�ļ���Ҳ��
{
	
    uint32 _cnt=0;
    uint16 temp;
    
	vcan_to_send[_cnt++]=0X03;
	vcan_to_send[_cnt++]=0xFC;
    
    temp = a;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = b;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = c;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = d;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = e;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = f;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    temp = g;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);
    
    
    temp = h;
	vcan_to_send[_cnt++]=BYTE1(temp);
	vcan_to_send[_cnt++]=BYTE0(temp);

	vcan_to_send[_cnt++]=0xFC;
	vcan_to_send[_cnt++]=0X03;	

    
    uart_putbuff (UART0, vcan_to_send, _cnt);
}