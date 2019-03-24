#include "STC15F2K60S2.h"
#include "interrupt.h"
volatile unsigned char CountNumber;

extern unsigned char BEEPON;
void Time0_Init1MS(void) // T0,T1������Ĭ��Ϊ12��Ƶ��33.1776MCPU����0.3617us  ��AUXR�Ĵ�����2Ϊ����  
{
	 TMOD |= 0X01;    //MODEL1 16λ��ʱ������ ,�������������ʱ��23.7ms
	 TL0=(65536-2764)%256;
   TH0=(65536-2764)/256;   //��ʱʱ��2764*0.3617us=20ms;  ��ʱ1s��ҪCountNumber=50��
	 ET0 =1;          //ʹ��t1�ж�
     //TR0=1;
	 EA=1;
}
void Time1_Init20MS(void) // T0,T1������Ĭ��Ϊ12��Ƶ��33.1776MCPU����0.3617us  ��AUXR�Ĵ�����2Ϊ����  
{
	 TMOD |= 0X10;    //MODEL1 16λ��ʱ������ ,�������������ʱ��23.7ms
	 TL1=(65536-55296)%256;
     TH1=(65536-55296)/256;   //��ʼֵ10240,��ʱʱ��55296*0.3617us=20ms;  ��ʱ1s��ҪCountNumber=50��
	 ET1 =1;          //ʹ��t1�ж�
     //TR1=1;
	 EA=1;
}



/***********************************************************************/
void Time0_IsrMS() interrupt 1 	   //��ʱ20ms,���Time1_Init()
{
	TL0=(65536-2764)%256;
	TH0=(65536-2764)/256; 
	if(BEEPON==1)   P42=~P42;
}	
/***********************************************************************/
void Time1_Isr20MS() interrupt 3 	   //��ʱ20ms,���Time1_Init()
{
//	TR1=0;
	TL1=15536%256;
	TH1=15536/256; 
	CountNumber++; 
 //   TR1=1;
}