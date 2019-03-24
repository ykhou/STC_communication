#include "STC15F2K60S2.h"
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "delay.h"
#include "zitai.h"
#include "iic.h"
#include "qiya_bmp085.h"
#include "cichang_hmc5883.h"
#include "jiasudu_adxl345.h"
#include "jiaosu_l3g4200.h"
/**
  zitai.c功能
 九轴加速度；（基于IIC协议）
 脉搏；		
*/
                      

sbit CG_MBO = P3^7;	 //输入	

void ZitaiIOInit(void)
{ 
/* 
P3M0 &=~(1<<7); //输入
P3M1 |=(1<<7); //输入	 
*/
Init_IIC();
}
void ZitaiMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"主界面",WHITE,RED);
}
void ZitTiaozTouch(void)
{
	unsigned char i=255;
	 ZitaiMFrame();
	 ZitaiIOInit();
	 Init_BMP085();
	 Init_HMC5883();
	 Init_ADXL345();
	 Init_L3G4200();
	 while(1)
	 {
			if(!Touch_IRQ()) //有触摸发生
			{
				if(Convert_Pos())	//得到坐标值
				{
					if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //下标签栏"主界面"
					{return;}

					}
			}
				if(i>20)
				{
				 bmp085Convert();	
						i=0;
				}
				HMC5883Convert();
				ADXL345Convert();
				L3G4200Convert();
				delayms(10);
				i++;
	   }
}