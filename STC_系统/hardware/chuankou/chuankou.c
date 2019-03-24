#include "STC15F2K60S2.h"
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "string.h"
#include "chuankou.h"
#include "font.h"
#include "delay.h"
#include "touch_main.h"

unsigned char OKFlag=0;
unsigned char LXSZFlag=0;   //������ʾ����
unsigned char CIDFlag=0,CIDFlag1=0;    //Caller Identification  ������ʾ
unsigned char RINGFlag=0;	//�绰����

extern unsigned char ldnum[12];	  //���ڴ���3Gģ�鴫����Ϣ�е��������
/**
  chuankou.c����
  ����1��ZIGBEE,P14(��չzigbee����ģ��)
  ����2������wifi-esp8266,max3232,OBD
*/


//����2
sbit CK_TXD = P1^0;	 //����
sbit CK_RXD = P1^1;
//����1
sbit UTXD = P3^0;	//����
sbit URXD = P3^1;
bit cktflg1=0;
unsigned char CKShowXNum1=2,CKShowYNum1=6,CKShowXNum2=16,CKShowYNum2=6;	//������ʾ����ĳ�ʼλ��

volatile unsigned char GetFlag1=0,GetFlag2=0;
volatile unsigned char BUSY1=0,BUSY2=0;	
unsigned char CK1Num=0,CK2Num=0;	
volatile unsigned char GetDataBuf1[100]='\0',GetDataBuf2[100]='\0';

void ChuankouIOInit(void)
{
P1M0 &=~(1<<0); //����
P1M1 |=(1<<0); //����

P3M0 &=~(1<<0); //����
P3M1 |=(1<<0); //����

P1M0 |=(1<<1); 
P1M1 &=~(1<<1);	

P3M0 |=(1<<1);
P3M1 &=~(1<<1);
}

void USART1_Init(void)
{
	SCON = 0X50;//01010000��01������ʽ1����8λUART�������ʿɱ䣩��01�����н���
	T2L=(65536-(F_CPU)/BAUD/4);//���ò�������װֵ
	T2H=(65536-(F_CPU)/BAUD/4)>>8;
	AUXR |= 0X15;	 //��ʱ��T2Ϊ�����ʷ�������T2Ϊ1Tģʽ��������ʱ��2
	ES =1; //����1�жϿ�
	EA =1;//�����ж�
}
void USART2_Init(void)
{
	S2CON = 0X50;//01010000��01������ʽ1����8λUART�������ʿɱ䣩��01�����н���
	T2L=(65536-(F_CPU)/BAUD/4);//���ò�������װֵ
	T2H=(65536-(F_CPU)/BAUD/4)>>8;
	AUXR |= 0X15;	 //��ʱ��T2Ϊ�����ʷ�������T2Ϊ1Tģʽ��������ʱ��2
	IE2 |=(1<<0);     //����2�жϿ�
	EA =1;//�����ж�
}


void SendData1(unsigned char dat)
{
  while(BUSY1);
  SBUF = dat;
	BUSY1 =1;    
}
void SendData2(unsigned char dat)
{
  while(BUSY2);
  S2BUF = dat;
	BUSY2 =1;    
}
void SendStr1(unsigned char *s1)
{
	while(*s1)
	{
	 	SendData1(*s1++);
	}
}
void SendStr2(unsigned char *s2)
{
	while(*s2)
	{
	 	SendData2(*s2++);
	}
}

void ChuankouMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,WHITE);
	
	LCD_DrawBox(4,300,118,20,RED);
	TFT_ShowString_W25(3,19,2,"�������",WHITE,RED);
	 
	LCD_DrawRectangle(128, 30,238, 296,WHITE);
	LCD_DrawRectangle(8,60, 125, 292,BLACK);  //���ݷ�����
	TFT_ClearArea(9,61,115,230,CYAN);
} 
/*
0��ʾ����1��
1��ʾ����1��
*/
void CHK1ButtonFrame(unsigned char a)
{
if(a==0) //����1 OFF
{
	TFT_ClearArea(20,20,85,45,WHITE);			 //6*8-13, 3*16-13,3*1+26,16+26
	TFT_ClearArea(20+5,20+5,85-10,45-10,BROWN);
	TFT_ClearArea(20+5+4,20+5+4,85-10-8,45-10-8,WHITE);
	TFT_ShowString_W25(5,2,2,"����1 ",BLUE,WHITE);
   ES =0;//���ж�
}else if(a==1) //����1  ON
{
	TFT_ClearArea(20,20,85,45,WHITE);			 //6*8-13, 3*16-13,3*1+26,16+26
	TFT_ClearArea(20+5,20+5,85-10,45-10,RED);
	TFT_ClearArea(20+5+4,20+5+4,85-10-8,45-10-8,WHITE);
	TFT_ShowString_W25(5,2,2,"����1 ",RED,WHITE);
	ES =1;//���ж�
}
}

void CHK2ButtonFrame(void)
{
	LCD_DrawRectangle(140,38,230,74,BLACK);
	TFT_ShowString_W25(19,3,2,"��ѯʱ��",BLUE,WHITE);
	LCD_DrawRectangle(140,86,230,122,BLACK);
	TFT_ShowString_W25(19,6,2,"ǿ������",BLUE,WHITE);
	LCD_DrawRectangle(140,134,230,170,BLACK);
	TFT_ShowString_W25(19,9,2,"�ź�����",BLUE,WHITE);
	LCD_DrawRectangle(140,182,230,218,BLACK);
	TFT_ShowString_W25(19,12,2,"������ʾ",BLUE,WHITE);
	LCD_DrawRectangle(140,230,230,266,BLACK);
	TFT_ShowString_W25(19,15,2,"һ������",BLUE,WHITE);
}

void peizhiTouch(void)
{
	unsigned char i;
	ChuankouIOInit();
	ChuankouMFrame();
	CHK1ButtonFrame(0);
	CHK2ButtonFrame();
	USART1_Init();
	USART2_Init();
	cktflg1=1;	   //��֤�������ý����ʱ�򣬴�������ֱ����ʾ��Ϣ��ֻ�д򿪴��ڰ�ť�󣬲Ż���ʾ��Ϣ
	while(1)
	{
		laidian(); 
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.y>300 && tp_pixlcd.y<320)	//�жϴ���λ��Ϊ�±�ǩ��"�������"
				{
					if(tp_pixlcd.x>4 && tp_pixlcd.x<122)
						{
							MainTouch();
							break;
						}
				}
				else if(tp_pixlcd.y<52&&tp_pixlcd.y>28)	  		 
				{
					if(tp_pixlcd.x<64&&tp_pixlcd.x>35)	 //����1 
					{
						cktflg1=~cktflg1;
					  if(cktflg1==1)
					  {			
							while(!Touch_IRQ());		  
							CHK1ButtonFrame(1);
						}	
					  else 
					  { 
							while(!Touch_IRQ());
							CHK1ButtonFrame(0);
						} 
						
					}	
				}
				else if(tp_pixlcd.y>38&&tp_pixlcd.y<74)	   //��ѯʱ��
				{
					if(tp_pixlcd.x<230&&tp_pixlcd.x>140)
				 	{	
					 	LCD_DrawBox(141,39,89,35,GREEN); 
						while(!Touch_IRQ());
						LCD_DrawBox(140,38,90,36,WHITE);
						LCD_DrawRectangle(140,38,230,74,BLACK);
						TFT_ShowString_W25(19,3,2,"��ѯʱ��",BLUE,WHITE);
						open5();
					} 
				}
				else if(tp_pixlcd.y>86&&tp_pixlcd.y<122)		 //ǿ������
				{
					if(tp_pixlcd.x<230&&tp_pixlcd.x>140)
				 	{
						LCD_DrawBox(141,87,89,35,GREEN); 
						while(!Touch_IRQ());
						LCD_DrawBox(140,86,90,36,WHITE); 
						LCD_DrawRectangle(140,86,230,122,BLACK);
						TFT_ShowString_W25(19,6,2,"ǿ������",BLUE,WHITE);
						open2(); 
					} 
				}
				else if(tp_pixlcd.y>134&&tp_pixlcd.y<170)    //��ѯ�ź�����
				{
					if(tp_pixlcd.x<230&&tp_pixlcd.x>140)
					{
					 	LCD_DrawBox(141,135,89,35,GREEN); 
						while(!Touch_IRQ());
						LCD_DrawBox(140,134,90,36,WHITE);
						LCD_DrawRectangle(140,134,230,170,BLACK);
						TFT_ShowString_W25(19,9,2,"�ź�����",BLUE,WHITE);
						open3();
					}
				}
				else if(tp_pixlcd.y>182&&tp_pixlcd.y<218)   //������ʾ
				{
					if(tp_pixlcd.x<230&&tp_pixlcd.x>140)
				 	{
					 	LCD_DrawBox(141,183,89,35,GREEN); 
						while(!Touch_IRQ());
						LCD_DrawBox(140,182,90,36,WHITE);
						LCD_DrawRectangle(140,182,230,218,BLACK);
						TFT_ShowString_W25(19,12,2,"������ʾ",BLUE,WHITE);
						open4(); 
						LXSZFlag=1;

					}
				}
				else if(tp_pixlcd.y>230&&tp_pixlcd.y<266)   //
				{
					if(tp_pixlcd.x<230&&tp_pixlcd.x>140)
				 	{
					 	LCD_DrawBox(141,231,89,35,GREEN); 
						while(!Touch_IRQ());
						LCD_DrawBox(140,230,90,36,WHITE);
						LCD_DrawRectangle(140,230,230,266,BLACK);
						TFT_ShowString_W25(19,15,2,"һ������",BLUE,WHITE);
						//LCD_DrawBox(141,39,89,35,GREEN);
						CHK1ButtonFrame(1);						
						//delayms(1000);
						//open1();
						//LCD_DrawBox(140,38,90,36,WHITE);
						//LCD_DrawRectangle(140,38,230,74,BLACK);
						//TFT_ShowString_W25(19,3,2,"�������",BLUE,WHITE);
						
						LCD_DrawBox(141,87,89,35,GREEN);
						delayms(1000);
						open2();
						LCD_DrawBox(140,86,90,36,WHITE); 
						LCD_DrawRectangle(140,86,230,122,BLACK);
						TFT_ShowString_W25(19,6,2,"ǿ������",BLUE,WHITE);
						
						LCD_DrawBox(141,135,89,35,GREEN); 
						delayms(1000);
						open3();
						LCD_DrawBox(140,134,90,36,WHITE);
						LCD_DrawRectangle(140,134,230,170,BLACK);
						TFT_ShowString_W25(19,9,2,"�ź�����",BLUE,WHITE);
						
						LCD_DrawBox(141,183,89,35,GREEN);
						delayms(1000);
						open4(); 
						LCD_DrawBox(140,182,90,36,WHITE);
						LCD_DrawRectangle(140,182,230,218,BLACK);
						TFT_ShowString_W25(19,12,2,"������ʾ",BLUE,WHITE);
						LXSZFlag=1;
					}
				}
			}				
		}

    //��GPRSģ�鴫����Ϣ��ʾ����Ļ��
		if(CK1Num>0 && cktflg1==1)
		{ 	
			i=0;
			while(GetDataBuf1[i])
			{	
				if(GetDataBuf1[i]>31)
					TFT_ShowWord(CKShowXNum1*8,CKShowYNum1*16,8,16,ASCII_816[GetDataBuf1[i]-0x20],BLACK,CYAN);
				else if(GetDataBuf1[i]=='\r'){i++;delayms(500); continue; }
				else if(GetDataBuf1[i]=='\n')  CKShowXNum1=13;			
				 
				if(++CKShowXNum1>13)
				{
				 	CKShowXNum1=2;
					if(++CKShowYNum1>17)
					{CKShowYNum1=5;TFT_ClearArea(9,61,115,230,CYAN);}
				 }
				if(++i>99) break;
			}
		}
		ES=0;     //����1�жϹ�
		memset(GetDataBuf1,0,100);CK1Num=0;
		ES=1;      //����1�жϿ�  	 
	}
}

void open1()
{
	 SendStr1("AT+COPS?\r\n");	
   //SendStr1("AT+COPS=?\r\n");	 //�������
}

void open2()
{
   SendStr1("AT+COPS=1,2,46000\r\n");	 //����
}

void open3()
{
   SendStr1("AT+CSQ\r\n");	 //�ź�����
}

void open4()
{
  SendStr1("AT+CLIP=1\r\n");	//������ʾ
	//SendStr1("AT+COLP=1\r\n");
}

void open5()
{
   SendStr1("AT+CCLK?\r\n");		 //��ѯʱ��
}

/*void open6()
{

   SendStr1("ATH\r\n");		 //�Ҷϣ��������ã�
}*/
unsigned char usartNum1=0,usartNum2=0,usartNum3=0;


void Uart1_Isr() interrupt 4 //using 0
{	
	unsigned char a;
 	if(RI)
	{
		RI =0;a=SBUF;
		if(a<128 && a>0)	//ֻ�����ַ���,��������
		{ 
			if(CK1Num>298){GetDataBuf1[CK1Num]='\0';CK1Num=0;}			
			GetDataBuf1[CK1Num++]=a;

			if(a=='R') usartNum1=1;
			else if(a=='I' && usartNum1==1) usartNum1=2;
			else if(a=='N' && usartNum1==2) usartNum1=3;
			else if(a=='G' && usartNum1==3){ usartNum1=0;RINGFlag=1;}
			else usartNum1=0;
			
			if(a=='+') usartNum2=1;
			else if(a=='C' && usartNum2==1) usartNum2=2;
			else if(a=='L' && usartNum2==2) usartNum2=3;
			else if(a=='I' && usartNum2==3) usartNum2=4;
			else if(a=='P' && usartNum2==4){ usartNum2=5;}
			else if(a==':' && usartNum2==5){ usartNum2=0;CIDFlag=1;}
			else usartNum2=0;
			if(CIDFlag==1)				
			{  				
				if(a=='\r')	return;
				if(a=='\n')
				{					
					CK2Num=0;
					CIDFlag=0;
					ldnum[0]=GetDataBuf2[3];		//��11��Ϊ��¼�������11λ�绰����
					ldnum[1]=GetDataBuf2[4];
					ldnum[2]=GetDataBuf2[5];
					ldnum[3]=GetDataBuf2[6];
					ldnum[4]=GetDataBuf2[7];
					ldnum[5]=GetDataBuf2[8];		
					ldnum[6]=GetDataBuf2[9];
					ldnum[7]=GetDataBuf2[10];
					ldnum[8]=GetDataBuf2[11];
					ldnum[9]=GetDataBuf2[12];
					ldnum[10]=GetDataBuf2[13];
					ldnum[11]='\0';
					CIDFlag1=1;
					return;
				}else
				{			             
					GetDataBuf2[CK2Num++]=a;	   //��CMT��T��ʼ�棬T���ڵ�һ��λ��
				}					
			}

			if(a=='O') usartNum3=1;
			else if(a=='K' && usartNum3==1) {usartNum3=0;OKFlag=1;}
			else usartNum3=0;
		}
	}
	if(TI)
	{
	 	TI =0;
		BUSY1=0;
	}
}
