#include <stdio.h>
#include <string.h>
#include "STC15F2K60S2.h"
#include "communicat.h"
#include "tft.h"
#include "txt.h"
#include "touch.h"
#include "delay.h"
#include "touch_main.h"
#include "chuankou.h"
#include "ruqing.h"

unsigned char STA[19]={0};
unsigned char ldnum[12]=0;	  //���ڴ���3Gģ�鴫����Ϣ�е��������
unsigned char phonenum[12]={0};	//ȥ�����
char time=0;
int restart = 0;
int G = 1;
extern unsigned char BEEPON;
extern unsigned char CIDFlag1;    //Caller Identification  ������ʾ
extern unsigned char RINGFlag;	//�绰����
extern unsigned char TouchNumber;

/************************ ͨ������***********************/ 
void CommunicatFrame(void)
{
	TFT_ClearONELINE(0,RED);
  TFT_ShowString(0,0,"CPU:STC15W4K48S4",WHITE,RED);
  TFT_ShowString_W25(20,0,2,"��˼��Ƽ�",WHITE,RED);
	
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	
	TFT_ShowString_W25(0,19,2,"������",WHITE,RED);

	LCD_DrawRectangle(16,16,224,56,BLUE);
	Draw_Circle(48,92,24,BLUE);//1
	Draw_Circle(120,92,24,BLUE);//2
	Draw_Circle(192,92,24,BLUE);//3
	Draw_Circle(48,152,24,BLUE);//4
	Draw_Circle(120,152,24,BLUE);//5
	Draw_Circle(192,152,24,BLUE);//6
	Draw_Circle(48,214,24,BLUE);//7
	Draw_Circle(120,214,24,BLUE);//8
	Draw_Circle(192,214,24,BLUE);//9
	Draw_Circle(48,276,24,BLUE);//C
	Draw_Circle(120,276,24,BLUE);//0
	Draw_Circle(192,276,24,BLUE);//G


	TFT_ShowString_W25(2,2,4,"1",BLACK,LIGHTBLUE);
  TFT_ShowString_W25(7,2,4,"2",BLACK,LIGHTBLUE);
  TFT_ShowString_W25(12,2,4,"3",BLACK,LIGHTBLUE);

  TFT_ShowString_W25(2,4,4,"4",BLACK,LIGHTBLUE);
  TFT_ShowString_W25(7,4,4,"5",BLACK,LIGHTBLUE);
  TFT_ShowString_W25(12,4,4,"6",BLACK,LIGHTBLUE);

	TFT_ShowString_W25(2,6,4,"7",BLACK,LIGHTBLUE);
	TFT_ShowString_W25(7,6,4,"8",BLACK,LIGHTBLUE);
	TFT_ShowString_W25(12,6,4,"9",BLACK,LIGHTBLUE);
	
	TFT_ShowString_W25(2,8,4,"C",BLACK,LIGHTBLUE);
	TFT_ShowString_W25(7,8,4,"0",BLACK,LIGHTBLUE);
	TFT_ShowString_W25(12,8,4,"D",BLACK,LIGHTBLUE);
}

/************************����***********************/
void CommunicatTouch(void)
{	
	restart = 0;
	TouchNumber=0;
	G = 1;
	CommunicatFrame();
	while(1)
	{
		
		while(1)
		{ 
			laidian(); 
			if(!Touch_IRQ())
			{
				if(Convert_Pos())	//�õ�����ֵ
				{
					TouchNumber=0;
					if(tp_pixlcd.x<48 && tp_pixlcd.y>304)  //�±�ǩ��"������"
					{
						restart=1;	
						break;
					}
					else if(tp_pixlcd.y>80 && tp_pixlcd.y<104)	 //��1��
					{
						if(tp_pixlcd.x>36 && tp_pixlcd.x<60)//1
							{TouchNumber=1;}
						else if(tp_pixlcd.x>108 && tp_pixlcd.x<132)//2
							{TouchNumber=2;}
						else if(tp_pixlcd.x>180 && tp_pixlcd.x<204)//3
							{TouchNumber=3;}

					}
					else if(tp_pixlcd.y>140 && tp_pixlcd.y<164)	 //��2�� 
					{
						if(tp_pixlcd.x>36 && tp_pixlcd.x<60)//4
							{TouchNumber=4;}
						else if(tp_pixlcd.x>108 && tp_pixlcd.x<132)//5
							{TouchNumber=5;}
						else if(tp_pixlcd.x>180 && tp_pixlcd.x<204)//6
							{TouchNumber=6;}
							
					}
					else if(tp_pixlcd.y>202 && tp_pixlcd.y<226)	 //��3�� 
					{
						if(tp_pixlcd.x>36 && tp_pixlcd.x<60)//7
							{TouchNumber=7;}
						else if(tp_pixlcd.x>108 && tp_pixlcd.x<132)//8
							{TouchNumber=8;}											
						else if(tp_pixlcd.x>180 && tp_pixlcd.x<204)//9
							{TouchNumber=9;}
					}
					else if(tp_pixlcd.y>264 && tp_pixlcd.y<288)	 //��4�� 
					{
						if(tp_pixlcd.x>36 && tp_pixlcd.x<60)//C
							{TouchNumber=10;}
						else if(tp_pixlcd.x>108 && tp_pixlcd.x<132)//0
							{TouchNumber=11;}											
						else if(tp_pixlcd.x>180 && tp_pixlcd.x<204)//D
							{TouchNumber=12;}	
					}
				}
			}

							
							
					if(TouchNumber==1)
					{
						Draw_Circle(48,92,24,RED);
						G++;
						break;
						
					}
					else if(TouchNumber==2)
					{
						Draw_Circle(120,92,24,RED);
						G++;
						break;
					}				
					else if(TouchNumber==3)
					{
						Draw_Circle(192,92,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==4)
					{
						Draw_Circle(48,152,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==5)
					{
						Draw_Circle(120,152,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==6)
					{
						Draw_Circle(192,152,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==7)
					{
						Draw_Circle(48,214,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==8)
					{
						Draw_Circle(120,214,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==9)
					{
						Draw_Circle(192,214,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==10)
					{
						Draw_Circle(48,276,24,RED);
						break;
					}
					else if(TouchNumber==11)
					{
						Draw_Circle(120,276,24,RED);
						G++;
						break;
					}
					else if(TouchNumber==12)
					{
						Draw_Circle(192,276,24,RED);
						break;
					}						
			}
		
		while(!Touch_IRQ());
		
				switch(TouchNumber)//�ɿ�
				{
					case 1:
					{
						Draw_Circle(48,92,24,BLUE);
						
						TFT_ShowString_W25(G,1,3,"1",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"1");
						break;
					}
					case 2:
					{
						Draw_Circle(120,92,24,BLUE);
						TFT_ShowString_W25(G,1,3,"2",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"2");
						break;
					}
					case 3:
					{
						Draw_Circle(192,92,24,BLUE);
						TFT_ShowString_W25(G,1,3,"3",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"3");
						break;
					}
					case 4:
					{
						Draw_Circle(48,152,24,BLUE);
						TFT_ShowString_W25(G,1,3,"4",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"4");
						break;
					}
					case 5:
					{
						Draw_Circle(120,152,24,BLUE);
						TFT_ShowString_W25(G,1,3,"5",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"5");
						break;
					}
					case 6:
					{
						Draw_Circle(192,152,24,BLUE);
						TFT_ShowString_W25(G,1,3,"6",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"6");
						break;
					}
					case 7:
					{
						Draw_Circle(48,214,24,BLUE);
						TFT_ShowString_W25(G,1,3,"7",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"7");
						break;
					}
					case 8:
					{
						Draw_Circle(120,214,24,BLUE);
						TFT_ShowString_W25(G,1,3,"8",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"8");
						break;
					}
					case 9:
					{
						Draw_Circle(192,214,24,BLUE);
						TFT_ShowString_W25(G,1,3,"9",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"9");
						break;
					}
					case 10:
					{
						Draw_Circle(48,276,24,BLUE);
						restart=1;
						callnum();
						break;
					}
					case 11:
					{
						Draw_Circle(120,276,24,BLUE);
						TFT_ShowString_W25(G,1,3,"0",MAGENTA,LIGHTBLUE);
						strcat(phonenum,"0");
						break;
					}
					case 12:
					{
						Draw_Circle(192,276,24,BLUE);
						if(G>1)
						{
							TFT_ShowString_W25(G,1,3," ",MAGENTA,LIGHTBLUE);
							phonenum[G-2]='\0';
							G--;
						}
						break;
					}
			}
		TouchNumber=0;
		if(restart == 1)
		{
			restart=0;
			MainTouch();
			break;
		}
	}
}

/************************����ͨ���ڽ���***********************/
extern unsigned char OKFlag;
//-----------------------------��3Gģ�鷢�Ͳ�������ָ��--------------------------------------//
void callnum()
{
	
  OKFlag=0;
	SendStr1("AT+SNFS=0\r\n");  //����Ƶͨ������Ϊ����                               ���ﻹӦ�ü��ϼ�ⳬʱ��ô��
	while(OKFlag!=1);
	strcpy(STA,"ATD");
 	strcat(STA,phonenum);
	strcat(STA,"\r\n");
	SendStr1(STA);
	time=0;
	calling();
}
//----------------------------------��������绰ʱ����--------------------------------------//
void calling()
{
	TFT_ClearArea(0,16,240,320-16,WHITE);
	TFT_ShowString_W25(9,7,2,"  CALLING...",BLUE,WHITE);
	LCD_DrawBox(50,200,150,50,RED);
	TFT_ShowString_W25(13,14,2,"�Ҷ�",WHITE,RED);
	TFT_ShowString_W25(9,4,2,phonenum,RED,WHITE);
	
	while(1)
	{  
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x>50&&tp_pixlcd.x<200)	  
			    {
					if(tp_pixlcd.y>200&&tp_pixlcd.y<250)	  		 
					{
			  		SendStr1("ATH\r\n");		//���͹Ҷ�ָ��
						memset(phonenum,'\0',sizeof(phonenum));
						memset(STA,'\0',sizeof(STA));
						break;		   //��ת��������				
					}
				}
			}
		}
	}	
}
//----------------------------------���н���--------------------------------------//
void receive(void)
{
	TFT_ClearArea(0,16,240,320-16,WHITE);
	TFT_ShowString_W25(11,5,2,"������...",BLUE,WHITE);
	TFT_ShowString_W25(9,7,2,ldnum,BLUE,WHITE);
	LCD_DrawBox(0,250,120,69,GREEN);
	LCD_DrawBox(121,250,119,69,RED);
	TFT_ShowString_W25(3,17,2,"ANSWER",WHITE,GREEN);
	TFT_ShowString_W25(19,17,2,"�Ҷ�",WHITE,RED);
	
	TR0=1;
	BEEPON=1;
	memset(ldnum,'\0',sizeof(ldnum));
	SendStr1("AT+SNFS=0\r\n");  //����Ƶͨ������Ϊ���� 
  
	while(1)
	{ 
		
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x>0&&tp_pixlcd.x<120)	  
				  if(tp_pixlcd.y>250&&tp_pixlcd.y<319)	  		 
					{
			 		    while(!Touch_IRQ());           //�ȴ������ɿ�����һ������Ҫ��
					    SendStr1("ATA\r\n");				   //��3Gģ�鷢�ͽ�����ָ��
							TFT_ShowString_W25(10,5,2,"CALLING...",BLUE,WHITE);
							TFT_ShowString_W25(9,7,2,ldnum,BLUE,WHITE);
							TR0=0;
							BEEPON=0;
						
					}
			  if(tp_pixlcd.x>120&&tp_pixlcd.x<239)
				{					
			 	  if(tp_pixlcd.y>250&&tp_pixlcd.y<319)	  		 
					{
						SendStr1("ATH\r\n");					//��3Gģ�鷢�͹Ҷϵ�ָ��
						TR0=0; 
						BEEPON=0;
						delayms(1000);
						RINGFlag=0;
						MainTouch();
						//break;
					}
				}
				
			}
		}
		//if(RINGFlag==0)
			//break;
	}
}	

//----------------------------------����ʱ��Ϣ��ȡ�жϲ���-------------------//
extern unsigned char LXSZFlag;   //������ʾ����
void laidian(void)
{
	if(RINGFlag==1&&CIDFlag1==1)
	{
			
		CIDFlag1=0;
		RINGFlag=0;
		receive();
						
	}
	else if(RINGFlag==1)
	{
		
		RINGFlag=0;
		receive();
						
	}
}