#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "delay.h"
#include "font.h"
#include "yaokong.h"
#include "STC15F2K60S2.h"
#include "try.h"

sbit JD1 = P3^5;
sbit JD2 = P5^1;
sbit CG_IRI =P4^1;
sbit CG_IRO =P3^2; //输入
/**
  yaokong.c功能
  继电器功能；
  红外接收和红外发射管；
*/

unsigned char xdata IRCOM[7];
volatile unsigned char IrFlag;
void YaokongIOInit(void)
{
	P3M0 |=(1<<5);	  //输入
	P3M1 &=~(1<<5);	   
	
	P5M0 |=(1<<1); 
	P5M1 &=~(1<<1); 
	
	JD1 =1;	//关闭继电器1
	JD2 =1;	//关闭继电器2
}
void JD1ON(void)
{
	JD1 =0;	//继电器1
}
void JD2ON(void)
{
	JD2 =0;	//继电器1	
}
void JD1OFF(void)
{
	JD1 =1;	//关闭继电器1
}
void JD2OFF(void)
{
	JD2 =1;	//关闭继电器1
}
void YaokongMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"主界面",WHITE,RED);	 

	TFT_ClearArea(55,125,112,53,BROWN);
	TFT_ClearArea(55+5,125+5,112-7,53-6,CYAN);
	TFT_ShowString_W25(9,9,2,"继电器测试",BROWN,CYAN);
}
void YaokongSFrame(void)
{
/********外框**********/
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"返 回",WHITE,RED);	 
}



/**功能：图像反转,按键效果
a=1分别表示"红外线测试"的反转色和正常色
a=2分别表示"继电器测试""的反转色和正常色
*/
void YaokongPressFrame(unsigned char a)
{
	if(a==2)
	{	
	TFT_ClearArea(55,125,112,53,RED);
	TFT_ClearArea(55+5,125+5,112-7,53-6,CYAN);
	TFT_ShowString_W25(9,9,2,"继电器测试",RED,CYAN);
	}
}

/*0表示按键关，1表示按键开*/
void JDButton1Frame(unsigned char a)
{
/**********按键效果*********/
if(a==0) //button1  OFF
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,BROWN);
	TFT_ClearArea(111,75+5+2,49,39,WHITE);
	TFT_ShowString_W25(15,6,2,"OFF",RED,WHITE);
	JD1=1;
}else if(a==1) //button1   ON
{
	TFT_ClearArea(55,75,112,53,WHITE);
	TFT_ClearArea(55+5,75+5,112-10,53-10,RED);
	TFT_ClearArea(62,75+5+2,49,39,WHITE);
	TFT_ShowString_W25(9,6,2,"O N",RED,WHITE);
	JD1=0;
}
}
/*0表示按键关，1表示按键开*/
void JDButton2Frame(unsigned char a)
{
if(a==0) //button2  OFF
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,BROWN);
	TFT_ClearArea(111,170+5+2,49,39,WHITE);
	TFT_ShowString_W25(15,12,2,"OFF",RED,WHITE);
	JD2=1;
}else if(a==1) //button1   ON
{
	TFT_ClearArea(55,170,112,53,WHITE);
	TFT_ClearArea(55+5,170+5,112-10,53-10,RED);
	TFT_ClearArea(62,170+5+2,49,39,WHITE);
	TFT_ShowString_W25(9,12,2,"O N",RED,WHITE);
	JD2=0;
}
}
void JDQFrame(void)
{
/**********内容*********/
JDButton1Frame(0);
JDButton2Frame(0);

/***********功能********************/

	while(1)
	{
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//得到坐标值
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //下标签栏"主界面"
				{JD1=JD2=1; return;			 }

				if(tp_pixlcd.x<160&&tp_pixlcd.x>111)	  		 
				{
					if(tp_pixlcd.y<121&&tp_pixlcd.y>82)	 //BUTTON1 ON 
					{JDButton1Frame(1);}
					else if(tp_pixlcd.y<216&&tp_pixlcd.y>177)//BUTTON2 ON	
					{JDButton2Frame(1);}
				}
				else if(tp_pixlcd.x<111&&tp_pixlcd.x>62)	  		
				{
					if(tp_pixlcd.y<121&&tp_pixlcd.y>82)	 //BUTTON1 OFF 
					{JDButton1Frame(0);}
					else if(tp_pixlcd.y<216&&tp_pixlcd.y>177)//BUTTON2 OFF	
					{JDButton2Frame(0);}						
				}
			}
		}
	}
}

void YaokJidTouch(void)
{
	  YaokongMFrame();
	  YaokongIOInit();
	  while(1)
	  {
		if(!Touch_IRQ()) //有触摸发生
		{
			if(Convert_Pos())	//得到坐标值
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //下标签栏"主界面"
				{return;}
				else if(tp_pixlcd.x>55&&tp_pixlcd.x<167)
				{
					if(tp_pixlcd.y>125&&tp_pixlcd.y<173) 
					{	
						YaokongPressFrame(2);delayms(500);
						YaokongSFrame(); //功能子界面 
						JDQFrame();
						YaokongMFrame(); //功能主界面 
					}
				}	 			
			}		
		} 
	}
}

