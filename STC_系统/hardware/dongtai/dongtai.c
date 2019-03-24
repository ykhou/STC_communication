
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"

#include "dongtai.h"
#include  "led_key.h"

/**
  dongtai.c功能
 震动传感CG_ZDO ；
 红外避障CG_BZO ；
*/
#include "STC15F2K60S2.h"

sbit CG_ZDO = P3^3;	 //输入	高电平有效
sbit CG_BZO = P3^6;	 //输入 低电平有效

void DongtaiIOInit(void)
{
P3M0 &=~(1<<2); //输入
P3M1 |=(1<<2); //输入

P3M0 &=~(1<<3); //输入
P3M1 |=(1<<3); //输入

P3M0 &=~(1<<6); //输入
P3M1 |=(1<<6); //输入

P4M0 |=(1<<1);
P4M1 &=~(1<<1);
LedKeyInit();
}
void DongtaiMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"主界面",WHITE,RED);
}
/*0表示关，1表示开*/
void ZDFrame(unsigned char a)
{

if(a==0) // OFF
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,BROWN);
	TFT_ClearArea(55+5+2,75+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,6,2,"震动检测",BROWN,WHITE);

}else if(a==1) //  ON
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,RED);
	TFT_ClearArea(62,75+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,6,2,"震动检测",RED,WHITE);
}
}
/*0表示关，1表示开*/
void HWBZFrame(unsigned char a)
{
if(a==0) // OFF
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,BROWN);
	TFT_ClearArea(62,170+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,12,2,"障碍检测",BROWN,WHITE);
}else if(a==1) // ON
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,RED);
	TFT_ClearArea(62,170+5+2,98,39,WHITE);
	TFT_ShowString_W25(10,12,2,"障碍检测",RED,WHITE);
}
}

void DongtJiancTouch(void)
{

	  DongtaiMFrame();
	  DongtaiIOInit();
	  ZDFrame(0);
	  HWBZFrame(0);
	
	  while(1)
	  {
		if(!Touch_IRQ()) //有触摸发生
		{
			if(Convert_Pos())	//得到坐标值
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //下标签栏"主界面"
				{LED3_OFF();LED4_OFF();return;}
			 }
		 }
		 if(CG_ZDO==1){LED3_ON();/*ZDFrame(1);*/}
		 else {LED3_OFF();/*ZDFrame(0);*/}
		 if(CG_BZO==0){LED4_ON();/*HWBZFrame(1);*/}
		 else {LED4_OFF();/*HWBZFrame(0);*/}
	   }
}