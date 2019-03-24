#include <stdio.h>
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "touch_main.h"
#include "paint.h"
#include "yaokong.h"	 
#include "chuankou.h"
#include "dongtai.h"
#include "huanjing.h"
#include "ruqing.h"
#include "zitai.h"
#include "communicat.h"
#include "phonebook.h"

extern unsigned char code phone[6050];
//extern unsigned char code phone[3025];
extern unsigned char code phonebooks[6050];
unsigned char TouchNumber=0;
/************************ 界面显示框架***********************/
void MainFrame(void)
{
	TFT_ClearONELINE(0,RED);
  TFT_ShowString(0,0,"CPU:STC15W4K48S4",WHITE,RED);
  TFT_ShowString_W25(20,0,2,"易思达科技",WHITE,RED);
	
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	//显示图标 及 图标对应功能语句
  TFT_ShowString_W25(2,7,2,"触摸画板",WHITE,MAGENTA);
  TFT_ShowString_W25(11,7,2,"遥控继电",WHITE,MAGENTA);
  TFT_ShowString_W25(20,7,2,"配置界面",WHITE,MAGENTA);

  TFT_ShowString_W25(2,13,2,"  拨号  ",WHITE,MAGENTA);
  TFT_ShowString_W25(11,13,2,"环境监测",WHITE,MAGENTA);
  TFT_ShowString_W25(20,13,2,"入侵报警",WHITE,MAGENTA);

	TFT_ShowString_W25(2,18,2," 电话簿 ",WHITE,MAGENTA);
	//TFT_ShowString_W25(11,18,2,"  拨号  ",WHITE,MAGENTA);
	//TFT_ShowString_W25(20,18,2," 电话簿 ",WHITE,MAGENTA);

	//打开和显示主界面图片
	
	DISP_BIN_PT_W25(17,50,55,55,0x619000,0x61BF44);		  //(45,50)
	DISP_BIN_PT_W25(92,50,55,55,0x61B000,0x61DF44);	  	//(140,50)
	DISP_BIN_PT_W25(167,50,55,55,0x61D000,0x61FF44);	  //(140,50)
    
  //DISP_BIN_PT_W25(17,140,55,55,0x61F000,0x621F44);	  //(45,140)
	DISP_BIN_PT(17,140,55,55,phone);
	DISP_BIN_PT_W25(92,140,55,55,0x621000,0x623F44);	  //(140,140)
  DISP_BIN_PT_W25(167,140,55,55,0x623000,0x625F44);	  //(140,50)
	
	DISP_BIN_PT(17,230,55,55,phonebooks);
  //DISP_BIN_PT_W25(17,230,55,55,0x625000,0x627F44);		//(45,230)
	//DISP_BIN_PT(92,230,55,55,phone);
	//DISP_BIN_PT(167,230,55,55,phonebooks);	
}



/************************ 触控功能***********************/ //	USART1_Init();USART2_Init();


void MainTouch(void)
{ 
		
	MainFrame();
	while(1)
	{
		laidian(); 
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//得到坐标值
			{
				TouchNumber=0;
				if(tp_pixlcd.y>50 && tp_pixlcd.y<105)	 //第一排 (30:30+55)
				{
					if(tp_pixlcd.x>17 && tp_pixlcd.x<72)//图一(17:17+55)
						{TouchNumber=1;}
					else if(tp_pixlcd.x>92 && tp_pixlcd.x<147)//图二(92:92+55)
						{TouchNumber=2;}
                    else if(tp_pixlcd.x>167 && tp_pixlcd.x<222)//图三(167:167+55)
						{TouchNumber=3;}

				}
				else if(tp_pixlcd.y>140 && tp_pixlcd.y<195)	 //第二排 (125:125+55)
				{
					if(tp_pixlcd.x>17 && tp_pixlcd.x<72)//图一(17:17+55)
						{TouchNumber=4;}
					else if(tp_pixlcd.x>92 && tp_pixlcd.x<147)//图二(92:92+55)
						{TouchNumber=5;}
                    else if(tp_pixlcd.x>167 && tp_pixlcd.x<222)//图三(167:167+55)
						{TouchNumber=6;}
				}
				else if(tp_pixlcd.y>230 && tp_pixlcd.y<285)	 //第三排 (220:220+55)
				{
					if(tp_pixlcd.x>17 && tp_pixlcd.x<72)//图一
						{TouchNumber=7;}
					/*else if(tp_pixlcd.x>92 && tp_pixlcd.x<147)//图二
						{TouchNumber=8;}											
					else if(tp_pixlcd.x>167 && tp_pixlcd.x<222)
						{TouchNumber=9;}*/
				}

				if(TouchNumber==1)
				{
					TFT_ShowString_W25(2,7,2,"触摸画板",WHITE,BLUE);
					break;
				}
				else if(TouchNumber==2)
				{
					TFT_ShowString_W25(11,7,2,"遥控继电",WHITE,BLUE);
					break;
				}				
				else if(TouchNumber==3)
				{
					TFT_ShowString_W25(20,7,2,"配置界面",WHITE,BLUE);
					break;
				}
				else if(TouchNumber==4)
				{
					//TFT_ShowString_W25(2,13,2,"",WHITE,BLUE);
					TFT_ShowString_W25(2,13,2,"  拨号  ",WHITE,BLUE);
					break;
				}
				else if(TouchNumber==5)
				{
					TFT_ShowString_W25(11,13,2,"环境监测",WHITE,BLUE);
					break;
				}
				else if(TouchNumber==6)
				{
					TFT_ShowString_W25(20,13,2,"入侵报警",WHITE,BLUE);
					break;
				}
				else if(TouchNumber==7)
				{
					//TFT_ShowString_W25(2,18,2,"姿态调整",WHITE,BLUE);
					TFT_ShowString_W25(2,18,2," 电话簿 ",WHITE,BLUE);
					break;
				}
			/*else if(TouchNumber==8)
			{
				  TFT_ShowString_W25(11,18,2,"  拨号  ",RED,BLUE);
				  break;
			}
			else if(TouchNumber==9)
			{
				  TFT_ShowString_W25(20,18,2," 电话簿 ",RED,BLUE);
				  break;
			}*/
			}
		}
	}

	while(!Touch_IRQ());//等待触摸松开

   	switch(TouchNumber)
	{
		case 1:
		{
			PaintTouch(); break;
		}
		case 2:
		{
			YaokJidTouch();break;
		}
 		case 3:
		{
			peizhiTouch();break;
	
		}
 		case 4:
		{
			 //DongtJiancTouch();
			CommunicatTouch();
			break;

		}
 		case 5:
		{
			HuanjJiancTouch();break;

		}
        case 6:
		{
			RuqBaojTouch();break;
	
		}
        case 7:
		{
			TxtTouch();
			//ZitTiaozTouch();
			break;
		}
			/*	case 8:
		{
			CommunicatTouch();break;
		}
				case 9:
		{
			break;
		}*/
		
	}
}


