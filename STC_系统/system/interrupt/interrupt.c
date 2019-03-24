#include "STC15F2K60S2.h"
#include "interrupt.h"
volatile unsigned char CountNumber;

extern unsigned char BEEPON;
void Time0_Init1MS(void) // T0,T1计数器默认为12分频（33.1776MCPU）即0.3617us  在AUXR寄存器高2为配置  
{
	 TMOD |= 0X01;    //MODEL1 16位定时计数器 ,计数器计满溢出时间23.7ms
	 TL0=(65536-2764)%256;
   TH0=(65536-2764)/256;   //定时时长2764*0.3617us=20ms;  定时1s需要CountNumber=50次
	 ET0 =1;          //使能t1中断
     //TR0=1;
	 EA=1;
}
void Time1_Init20MS(void) // T0,T1计数器默认为12分频（33.1776MCPU）即0.3617us  在AUXR寄存器高2为配置  
{
	 TMOD |= 0X10;    //MODEL1 16位定时计数器 ,计数器计满溢出时间23.7ms
	 TL1=(65536-55296)%256;
     TH1=(65536-55296)/256;   //初始值10240,定时时长55296*0.3617us=20ms;  定时1s需要CountNumber=50次
	 ET1 =1;          //使能t1中断
     //TR1=1;
	 EA=1;
}



/***********************************************************************/
void Time0_IsrMS() interrupt 1 	   //定时20ms,详见Time1_Init()
{
	TL0=(65536-2764)%256;
	TH0=(65536-2764)/256; 
	if(BEEPON==1)   P42=~P42;
}	
/***********************************************************************/
void Time1_Isr20MS() interrupt 3 	   //定时20ms,详见Time1_Init()
{
//	TR1=0;
	TL1=15536%256;
	TH1=15536/256; 
	CountNumber++; 
 //   TR1=1;
}