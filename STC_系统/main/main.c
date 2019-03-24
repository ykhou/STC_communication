#include "STC15F2K60S2.h"
#include  "delay.h"				  //延时函数
#include  "touch.h"
#include  "tft.h"			  //TFT IC底层驱
#include  "txt.h"
#include  "bin.h"
#include  "touch_main.h"
#include "led_key.h"
#include "flash.h"
#include "fontcheck.h"
void main()
{  	
	P0M0 = 0X00;  P0M1 = 0X00;
	P1M0 = 0X00;  P1M1 = 0X00;
	P2M0 = 0X00;  P2M1 = 0X00;
	P3M0 = 0X00;  P3M1 = 0X00;
	P4M0 = 0X00;  P4M1 = 0X00;
	P5M0 = 0X00;  P5M1 = 0X00;
  P6M0 = 0X00;  P6M1 = 0X00;
	P7M0 = 0X00;  P7M1 = 0X00;
	P41=0; //关闭红外发射管

	TFT_Init();                //tft初始化
	Touch_InitSpi();
	InitSpi();
	ftinfoInt();	
 
	while(1)
	{
		MainTouch();	   //启动 触摸屏 操作界面	
	}	
 	
}

