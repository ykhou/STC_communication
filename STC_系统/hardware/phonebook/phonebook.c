#include <stdio.h>
#include <string.h>
#include "txt.h"
#include "STC15F2K60S2.h"
#include "communicat.h"
#include "tft.h"
#include "touch.h"
#include "delay.h"
#include "touch_main.h"
#include "chuankou.h"
#include "ruqing.h"
#include "phonebook.h"


extern unsigned char TouchNumber;
extern int restart;
extern unsigned char phonenum[12];	//去电号码
//unsigned chzr Rain[12]={1,5,8,2,7,5,9,8,6,7,5};
int j = 0 ;

void TxtFrame(void)
{
	
	TFT_ClearONELINE(0,RED);
  TFT_ShowString(0,0,"CPU:STC15W4K48S4",WHITE,RED);
  TFT_ShowString_W25(20,0,2,"易思达科技",WHITE,RED);
	
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	
	TFT_ShowString_W25(0,19,2,"主界面",WHITE,RED);
	TFT_ShowString_W25(1,3,3,"陈紫康",WHITE,MAGENTA);
	TFT_ShowString_W25(8,3,3,"15827598675",WHITE,MAGENTA);
	TFT_ShowString_W25(1,5,3,"侯永康",WHITE,MAGENTA);
	TFT_ShowString_W25(8,5,3,"15827077171",WHITE,MAGENTA);
}

void TxtTouch(void)
{
	restart = 0;
	memset(phonenum,'\0',sizeof(phonenum));
	TxtFrame();
	while(1)
	{
		while(1)
		{
			laidian(); 			
			if(!Touch_IRQ())
			{
				if(Convert_Pos())	//得到坐标值
				{
					TouchNumber=0;
					if(tp_pixlcd.x<48 && tp_pixlcd.y>304)  //下标签栏"主界面"
					{
						restart=1;	
						break;
					}
					else if(tp_pixlcd.y>48 && tp_pixlcd.y<85)	 //第1排
					{
						if(tp_pixlcd.x>30 && tp_pixlcd.x<96)//1
							{
								TouchNumber=1;
								strcat(phonenum,"15827598675");	
							}
					}
					else if(tp_pixlcd.y>110 && tp_pixlcd.y<147)	 //第2排
					{
						if(tp_pixlcd.x>30 && tp_pixlcd.x<96)//
							{
								TouchNumber=1;
								strcat(phonenum,"15827077171");						
							}
					}
				}
			}
			if(TouchNumber==1)
			{
				restart=1;
				callnum();
				break;
			}
			
		}
		if(restart == 1)
		{
			restart = 0;
			MainTouch();
			break;
		}
	}
}