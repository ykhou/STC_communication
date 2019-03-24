
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"

#include "ruqing.h"
#include "interrupt.h"
#include "delay.h"
#include  "led_key.h"
#include "STC15F2K60S2.h"

sbit CG_FMQ = P4^2;	 //输出  脉冲	
sbit CG_RSO = P4^3;	//输入 高电平有效
/**
  ruqing.c功能
 蜂鸣器；
 人体红外热释电；
*/
unsigned char BEEPON=0;

void RuqingIOInit(void)
{  
P4M0 |=(1<<2);
P4M1 &=~(1<<2);	

P4M0 &=~(1<<3); //输入
P4M1 |=(1<<3); //输入

CG_FMQ =0;//关闭蜂鸣器
}  

void Beep_On(void)
{
	CG_FMQ =1;//蜂鸣器
}
void Beep_Off(void)
{
	CG_FMQ =0;//关闭蜂鸣器
}
void RuqingMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"主界面",WHITE,RED);
}
/*0表示关，1表示开*/
void FMQFrame(unsigned char a)
{
if(a==0) // OFF
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,BROWN);
	TFT_ClearArea(55+5+2,75+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,6,2,"蜂鸣器OFF",BROWN,WHITE);
//	BEEPON=0;
}else if(a==1) //  ON
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,RED);
	TFT_ClearArea(62,75+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,6,2,"蜂鸣器ON",RED,WHITE);
//	BEEPON=1;
}
}
/*0表示关，1表示开*/
void RSDFrame(unsigned char a)
{
if(a==0) // OFF
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,BROWN);
	TFT_ClearArea(62,170+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,12,2,"入侵OFF",BROWN,WHITE);
//	BEEPON=0;
}else if(a==1) // ON
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,RED);
	TFT_ClearArea(62,170+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,12,2,"入侵ON",RED,WHITE);
//	BEEPON=1;
}
}



void RuqBaojTouch(void)
{
	 bit RQflag1=0,RQflag2=0;
	  RuqingMFrame();
	  FMQFrame(0);
	  RSDFrame(0);
	  RuqingIOInit();
	  Time0_Init1MS();
	  LedKeyInit();
	  TR0=1;//开始计数
	  BEEPON=0;

	  while(1)
	  {
		if(!Touch_IRQ()) //有触摸发生
		{
			if(Convert_Pos())	//得到坐标值
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //下标签栏"主界面"
				{TR0=0;ET0 =0;EA=0;CountNumber=0;return;}

				if(tp_pixlcd.x<160&&tp_pixlcd.x>62)	  		 
				{
					if(tp_pixlcd.y<121&&tp_pixlcd.y>82)	 //BUTTON1 OFF 
					{
						while(!Touch_IRQ());//等待触摸松开
						RQflag1=~RQflag1;
						if(RQflag1==1)FMQFrame(1);
						else FMQFrame(0);
					}
 					if(tp_pixlcd.y<216&&tp_pixlcd.y>177)//BUTTON2 OFF	
					{
						while(!Touch_IRQ());//等待触摸松开
						RQflag2=~RQflag2;
						if(RQflag2==1)RSDFrame(1);
						else RSDFrame(0);
					
					}
			
				}

			 }
		 }
		 
		if(RQflag1==1 && RQflag2==1 && CG_RSO==1){BEEPON=1;LED3_ON();LED4_ON();}  
		else {LED3_OFF();LED4_OFF();BEEPON=0;}	  
	   }

}