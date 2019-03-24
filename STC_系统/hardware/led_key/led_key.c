
#include  "delay.h"				  //延时函数
#include  "led_key.h"	
#include "STC15F2K60S2.h"
sbit LED_D3 = P6^6;	//CG_BY2 ,CK_CE
sbit LED_D4  =P6^7;	//CG_BY3 ,CK_CSN
sbit KEY  =P3^4;

void LedKeyInit(void)
{
P3M1 &=~(1<<4);
P3M1 |=(1<<4);	  //key输入	 按键不外接上拉电阻就不要配置,不然时钟判断是低电平

P6M0 |=(1<<6)|(1<<7); //LED输出
P6M1 &=~((1<<6)|(1<<7));

}
void LED3_ON(void)
{
	LED_D3=0;
}
void LED4_ON(void)
{
	LED_D4=0;
}
void LED3_OFF(void)
{
	LED_D3=1;
}
void LED4_OFF(void)
{
	LED_D4=1;
}
void LedMode(unsigned char a)
{
 	 switch(a)
	 {
	 	case 0:	LED_D3 =0;LED_D4 =1;  break;
		case 1:	LED_D3 =1;LED_D4 =0;  break;
		case 2:	LED_D3 =0;LED_D4 =0;  break;
		case 3:	LED_D3 =1;LED_D4 =1;  break;
	 }
	 
}

void KeyTest(void)
{
	static unsigned char ledmod=0;

	if(KEY==0)
	{ 
		delayms(100);
		if(KEY==0)
		{	
			if(ledmod++>4) ledmod=0;
		}
	}
	LedMode(ledmod);	 
}