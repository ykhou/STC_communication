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
sbit CG_IRO =P3^2; //����
/**
  yaokong.c����
  �̵������ܣ�
  ������պͺ��ⷢ��ܣ�
*/

unsigned char xdata IRCOM[7];
volatile unsigned char IrFlag;
void YaokongIOInit(void)
{
	P3M0 |=(1<<5);	  //����
	P3M1 &=~(1<<5);	   
	
	P5M0 |=(1<<1); 
	P5M1 &=~(1<<1); 
	
	JD1 =1;	//�رռ̵���1
	JD2 =1;	//�رռ̵���2
}
void JD1ON(void)
{
	JD1 =0;	//�̵���1
}
void JD2ON(void)
{
	JD2 =0;	//�̵���1	
}
void JD1OFF(void)
{
	JD1 =1;	//�رռ̵���1
}
void JD2OFF(void)
{
	JD2 =1;	//�رռ̵���1
}
void YaokongMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"������",WHITE,RED);	 

	TFT_ClearArea(55,125,112,53,BROWN);
	TFT_ClearArea(55+5,125+5,112-7,53-6,CYAN);
	TFT_ShowString_W25(9,9,2,"�̵�������",BROWN,CYAN);
}
void YaokongSFrame(void)
{
/********���**********/
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"�� ��",WHITE,RED);	 
}



/**���ܣ�ͼ��ת,����Ч��
a=1�ֱ��ʾ"�����߲���"�ķ�תɫ������ɫ
a=2�ֱ��ʾ"�̵�������""�ķ�תɫ������ɫ
*/
void YaokongPressFrame(unsigned char a)
{
	if(a==2)
	{	
	TFT_ClearArea(55,125,112,53,RED);
	TFT_ClearArea(55+5,125+5,112-7,53-6,CYAN);
	TFT_ShowString_W25(9,9,2,"�̵�������",RED,CYAN);
	}
}

/*0��ʾ�����أ�1��ʾ������*/
void JDButton1Frame(unsigned char a)
{
/**********����Ч��*********/
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
/*0��ʾ�����أ�1��ʾ������*/
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
/**********����*********/
JDButton1Frame(0);
JDButton2Frame(0);

/***********����********************/

	while(1)
	{
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //�±�ǩ��"������"
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
		if(!Touch_IRQ()) //�д�������
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //�±�ǩ��"������"
				{return;}
				else if(tp_pixlcd.x>55&&tp_pixlcd.x<167)
				{
					if(tp_pixlcd.y>125&&tp_pixlcd.y<173) 
					{	
						YaokongPressFrame(2);delayms(500);
						YaokongSFrame(); //�����ӽ��� 
						JDQFrame();
						YaokongMFrame(); //���������� 
					}
				}	 			
			}		
		} 
	}
}

