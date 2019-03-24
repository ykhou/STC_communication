#include <intrins.h>
#include <stdio.h>
#include "STC15F2K60S2.h"	
#include "delay.h"
#include "tft.h"
#include "font.h"
#include "touch.h"
/*	
	*������
	P4.0-----------T_IRQ	����IO����
	P4.7-----------T_DO	    ����IO����
	P6.1-----------T_CLK
	P6.2-----------T_CS
	P6.3-----------T_DIN
*/
/*********************TFT���������Ŷ���**********************************/
sbit TOUCH_IRQ =P4^0;   //��ⴥ������Ӧ�ź�
sbit TOUCH_DO  =P4^7;	
sbit TOUCH_CLK =P6^1;   
sbit TOUCH_CS  =P6^2;
sbit TOUCH_DIN =P6^3;	
						
//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
unsigned int vx=7839,vy=5599;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
unsigned int chx=93,chy=1949;   //Ĭ��LCD����(0,0)ʱ��Ӧ�Ĵ�����AD��ʼֵ
//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����

//struct tp_pix_  tp_pixad,tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ   
TP16_pix xdata tp_pixad;
TP16_pix xdata tp_pixlcd;


unsigned char Touch_IRQ(void)
{
	return TOUCH_IRQ;  
}    
//**********************************************************
void Touch_InitSpi(void)                                     //SPI��ʼ��
{
	P4M1 |=(1<<0) |(1<<7);	 //����
	P6M0 |=(1<<1)|(1<<2) |(1<<3);
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPIд����
{
	unsigned char count;
	for(count=0;count<8;count++)
	{
		num<<=1;
		TOUCH_DIN=CY;
		TOUCH_CLK=0; _nop_();_nop_();_nop_();                //��������Ч
		TOUCH_CLK=1; _nop_();_nop_();_nop_();
	}
}
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI ������
{
	unsigned char count=0;
	unsigned int Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		TOUCH_CLK=1; _nop_();_nop_();_nop_();                //�½�����Ч
		TOUCH_CLK=0; _nop_();_nop_();_nop_();
		if(TOUCH_DO)
		{
			Num|=1;
		}
		
	}
	return(Num);
}	
//��7846/7843/XPT2046/UH7843/UH7846��ȡadcֵ	 CMD���0x90��Y���� 0xd0��X����
unsigned int ADS_Read_AD(unsigned char CMD)          
{
	unsigned int l;
	TOUCH_CS=0;
	WriteCharTo7843(CMD);        //�Ϳ����ּ��ò�ַ�ʽ��X���� ��ϸ����й�����
//	TOUCH_CLK=1; _nop_();_nop_();_nop_();_nop_();
//	TOUCH_CLK=0; _nop_();_nop_();_nop_();_nop_();
	l=ReadFromCharFrom7843();
	TOUCH_CS=1;
	return l;
}		   
//��ȡһ������ֵ
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
#define READ_TIMES 15 //��ȡ����   15
#define LOST_VAL 5	  //����ֵ 5
unsigned int ADS_Read_XY(unsigned char xy)
{
	unsigned char i, j;
	unsigned int xdata buf[READ_TIMES];
	unsigned int sum=0;
	unsigned int temp;
	for(i=0;i<READ_TIMES;i++)
	{				 
		buf[i]=ADS_Read_AD(xy);	    
	}				    
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
//	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//���˲��������ȡ
//��Сֵ��������100.

unsigned char Read_ADS(unsigned int *x,unsigned int *y)
{
	unsigned int xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;//����ʧ��
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//2�ζ�ȡADS7846,������ȡ2����Ч��ADֵ,�������ε�ƫ��ܳ���
//50,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
#define ERR_RANGE 20 //��Χ 
unsigned char Read_ADS2(unsigned int *x,unsigned int *y) 
{
	unsigned int x1,y1;
 	unsigned int x2,y2;
 	unsigned char flag;    
    flag=Read_ADS(&x1,&y1); //����һ��  
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	//���ڶ���
    if(flag==0)return(0);   

    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-ERR_RANGE��
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }else return 0;	  
} 
//��ȷ��ȡһ������,У׼��ʱ���õ�	   
/*unsigned char Read_TP_Once(void)
{
	unsigned char re=0;
	unsigned int x1,y1;
	while(re==0)
	{
		while(!Read_ADS2(&tp_pixad.x,&tp_pixad.y));
		delayms(10);
		while(!Read_ADS2(&x1,&y1));
		if(tp_pixad.x==x1&&tp_pixad.y==y1)
		{
			re=1; 
		}
	} 
	return re;
}  */
//////////////////////////////////////////////////
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
void Drow_Touch_Point(unsigned int x,unsigned int y)
{
	LCD_DrawLine(x-12,y,x+13,y,RED);//����	12+13=25�㳤
	LCD_DrawLine(x,y-12,x,y+13,RED);//����
	LCD_DrawPoint_big(x+1,y+1,1,BLUE);
	LCD_DrawPoint_big(x-1,y+1,1,BLUE);
	LCD_DrawPoint_big(x+1,y-1,1,BLUE);
	LCD_DrawPoint_big(x-1,y-1,1,BLUE);
//	Draw_Circle(x,y,6,BLUE);//������Ȧ
}	  
//ת�����
//���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
//chx,chyΪLCD(0,0)����ADֵ
//vx,vyΪÿ�����ص�ռn��ADֵ
//�㷨��ʾ����ADֵ/n=��������
unsigned char Convert_Pos(void)
{		 	 
	unsigned char l=0; 
	if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))
	{
		l=1;
		tp_pixlcd.x=tp_pixad.x>chx?((unsigned long)tp_pixad.x-(unsigned long)chx)*1000/vx:((unsigned long)chx-(unsigned long)tp_pixad.x)*1000/vx;
		tp_pixlcd.y=tp_pixad.y>chy?((unsigned long)tp_pixad.y-(unsigned long)chy)*1000/vy:((unsigned long)chy-(unsigned long)tp_pixad.y)*1000/vy;
	}
	return l;
}	   
//������У׼����
//�õ��ĸ�У׼����
#define tp_pianyi 50   //У׼����ƫ����	
#define tp_xiaozhun 300   //У׼����
void Touch_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
	unsigned int chx1,chx2,chy1,chy2;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ
	TP32_pix xdata p[4];
	unsigned char  cnt=0;
//	unsigned char  showzifu[6];	 
	TFT_ClearByColor(WHITE);//���� 

	Drow_Touch_Point(tp_pianyi,tp_pianyi);//��У׼��1 

	while(1)
	{
		if(TOUCH_IRQ==0)//����������
		{	
			
			if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))//�õ����ΰ���ֵ
			{  								   
				p[cnt].x=tp_pixad.x;
				p[cnt].y=tp_pixad.y;
				cnt++; 
			}
			switch(cnt)
			{			   
				case 1:
					TFT_ClearByColor(WHITE);//���� 
					while(!TOUCH_IRQ)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,tp_pianyi);//����2
					break;
				case 2:
					TFT_ClearByColor(WHITE);//���� 
					while(!TOUCH_IRQ)  //�ȴ�����
					{
					}
					Drow_Touch_Point(tp_pianyi,LCD_H-tp_pianyi-1);//����3
					break;
				case 3:
					TFT_ClearByColor(WHITE);//���� 
					while(!TOUCH_IRQ)  //�ȴ�����
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,LCD_H-tp_pianyi-1);//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		   	TFT_ClearByColor(WHITE);//���� 
				   	while(!TOUCH_IRQ);  //�ȴ�����
					//���������� �� LCD���� ��Ӧ
					//vx1����һ�����ص��ж��ٸ�ADֵ
			   		vx1=p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
				 	//����ʼ�㣨0,0��x�����Ӧ��ADֵ
				 	chx1=p[1].x>p[0].x?p[0].x-(vx1*tp_pianyi)/1000:p[0].x+(vx1*tp_pianyi)/1000;
				   	
					vy1=p[2].y>p[0].y?(p[2].y-p[0].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[0].y-p[2].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy1=p[2].y>p[0].y?p[0].y-(vy1*tp_pianyi)/1000:p[0].y+(vy1*tp_pianyi)/1000; 
					
					vx2=p[3].x>p[2].x?(p[3].x-p[2].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[2].x-p[3].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
					chx2=p[3].x>p[2].x?p[2].x-(vx2*tp_pianyi)/1000:p[2].x+(vx2*tp_pianyi)/1000;
				   	
					vy2=p[3].y>p[1].y?(p[3].y-p[1].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi):(p[1].y-p[3].y-1)*1000/(LCD_H-tp_pianyi-tp_pianyi);
					chy2=p[3].y>p[1].y?p[1].y-(vy2*tp_pianyi)/1000:p[1].y+(vy2*tp_pianyi)/1000; 

		
					if((vx1>vx2&&vx1>vx2+tp_xiaozhun)||(vx1<vx2&&vx1<vx2-tp_xiaozhun)||(vy1>vy2&&vy1>vy2+tp_xiaozhun)||(vy1<vy2&&vy1<vy2-tp_xiaozhun))
					{
						cnt=0;
						TFT_ClearByColor(WHITE);//���� 
						Drow_Touch_Point(tp_pianyi,tp_pianyi);//����1 
						continue;
					}
					vx=(vx1+vx2)/2;vy=(vy1+vy2)/2;
					chx=(chx1+chx2)/2;chy=(chy1+chy2)/2;	
																
					//��ʾУ׼��Ϣ
					TFT_ClearByColor(WHITE);//���� 

/*	 
					sprintf(showzifu,"%f",vx1);			
					TFT_ShowString(0,0,"VX1:",BLUE,WHITE);TFT_ShowString(4,0,showzifu,RED,WHITE);					
					sprintf(showzifu,"%f",vy1);	
					TFT_ShowString(0,1,"Vy1:",BLUE,WHITE);TFT_ShowString(4,1,showzifu,RED,WHITE);
					sprintf(showzifu,"%d",chx1);						
					TFT_ShowString(0,2,"CHX1:",BLUE,WHITE);TFT_ShowString(5,2,showzifu,RED,WHITE);
					sprintf(showzifu,"%d",chy1);					
					TFT_ShowString(0,3,"CHY1:",BLUE,WHITE);TFT_ShowString(5,3,showzifu,RED,WHITE);

					sprintf(showzifu,"%f",vx2);				
					TFT_ShowString(0,4,"VX2:",BLUE,WHITE);TFT_ShowString(4,4,showzifu,RED,WHITE);					
					sprintf(showzifu,"%f",vy2);	
					TFT_ShowString(0,5,"Vy2:",BLUE,WHITE);TFT_ShowString(4,5,showzifu,RED,WHITE);					
					sprintf(showzifu,"%d",chx2);	 
					TFT_ShowString(0,6,"CHX2:",BLUE,WHITE);TFT_ShowString(5,6,showzifu,RED,WHITE);					
				    sprintf(showzifu,"%d",chy2);	
					TFT_ShowString(0,7,"CHY2:",BLUE,WHITE);TFT_ShowString(5,7,showzifu,RED,WHITE);
				
					sprintf(showzifu,"%d",vx);				
					TFT_ShowString(0,8,"VX:",BLUE,WHITE);TFT_ShowString(4,8,showzifu,RED,WHITE);					
					sprintf(showzifu,"%d",vy);	
					TFT_ShowString(0,9,"Vy:",BLUE,WHITE);TFT_ShowString(4,9,showzifu,RED,WHITE);					
					sprintf(showzifu,"%d",chx);	 
					TFT_ShowString(0,10,"CHX:",BLUE,WHITE);TFT_ShowString(5,10,showzifu,RED,WHITE);					
				    sprintf(showzifu,"%d",chy);	 
					TFT_ShowString(0,11,"CHY:",BLUE,WHITE);TFT_ShowString(5,11,showzifu,RED,WHITE);

			    	TFT_ShowString(0,14,"Adjust OK!  Touch Anywhere To Continue",BLUE,RED);										  
					Read_TP_Once(); //�ȴ�����������
	  */
					TFT_ClearByColor(WHITE);//����
					return;//У�����				 
			}
		}
	} 
}

