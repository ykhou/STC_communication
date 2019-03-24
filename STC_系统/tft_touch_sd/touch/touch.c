#include <intrins.h>
#include <stdio.h>
#include "STC15F2K60S2.h"	
#include "delay.h"
#include "tft.h"
#include "font.h"
#include "touch.h"
/*	
	*触摸屏
	P4.0-----------T_IRQ	主机IO输入
	P4.7-----------T_DO	    主机IO输入
	P6.1-----------T_CLK
	P6.2-----------T_CS
	P6.3-----------T_DIN
*/
/*********************TFT触摸屏引脚定义**********************************/
sbit TOUCH_IRQ =P4^0;   //检测触摸屏响应信号
sbit TOUCH_DO  =P4^7;	
sbit TOUCH_CLK =P6^1;   
sbit TOUCH_CS  =P6^2;
sbit TOUCH_DIN =P6^3;	
						
//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
unsigned int vx=7839,vy=5599;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
unsigned int chx=93,chy=1949;   //默认LCD坐标(0,0)时对应的触摸板AD起始值
//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数

//struct tp_pix_  tp_pixad,tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
TP16_pix xdata tp_pixad;
TP16_pix xdata tp_pixlcd;


unsigned char Touch_IRQ(void)
{
	return TOUCH_IRQ;  
}    
//**********************************************************
void Touch_InitSpi(void)                                     //SPI初始化
{
	P4M1 |=(1<<0) |(1<<7);	 //输入
	P6M0 |=(1<<1)|(1<<2) |(1<<3);
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI写数据
{
	unsigned char count;
	for(count=0;count<8;count++)
	{
		num<<=1;
		TOUCH_DIN=CY;
		TOUCH_CLK=0; _nop_();_nop_();_nop_();                //上升沿有效
		TOUCH_CLK=1; _nop_();_nop_();_nop_();
	}
}
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI 读数据
{
	unsigned char count=0;
	unsigned int Num=0;
	for(count=0;count<12;count++)
	{
		Num<<=1;		
		TOUCH_CLK=1; _nop_();_nop_();_nop_();                //下降沿有效
		TOUCH_CLK=0; _nop_();_nop_();_nop_();
		if(TOUCH_DO)
		{
			Num|=1;
		}
		
	}
	return(Num);
}	
//从7846/7843/XPT2046/UH7843/UH7846读取adc值	 CMD命令：0x90读Y坐标 0xd0读X坐标
unsigned int ADS_Read_AD(unsigned char CMD)          
{
	unsigned int l;
	TOUCH_CS=0;
	WriteCharTo7843(CMD);        //送控制字即用差分方式读X坐标 详细请见有关资料
//	TOUCH_CLK=1; _nop_();_nop_();_nop_();_nop_();
//	TOUCH_CLK=0; _nop_();_nop_();_nop_();_nop_();
	l=ReadFromCharFrom7843();
	TOUCH_CS=1;
	return l;
}		   
//读取一个坐标值
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
#define READ_TIMES 15 //读取次数   15
#define LOST_VAL 5	  //丢弃值 5
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
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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
//带滤波的坐标读取
//最小值不能少于100.

unsigned char Read_ADS(unsigned int *x,unsigned int *y)
{
	unsigned int xtemp,ytemp;			 	 		  
	xtemp=ADS_Read_XY(CMD_RDX);
	ytemp=ADS_Read_XY(CMD_RDY);	 									   
	if(xtemp<100||ytemp<100)return 0;//读数失败
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//2次读取ADS7846,连续读取2次有效的AD值,且这两次的偏差不能超过
//50,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
#define ERR_RANGE 20 //误差范围 
unsigned char Read_ADS2(unsigned int *x,unsigned int *y) 
{
	unsigned int x1,y1;
 	unsigned int x2,y2;
 	unsigned char flag;    
    flag=Read_ADS(&x1,&y1); //读第一次  
    if(flag==0)return(0);
    flag=Read_ADS(&x2,&y2);	//读第二次
    if(flag==0)return(0);   

    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-ERR_RANGE内
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)>>1;
        *y=(y1+y2)>>1;		
        return 1;
    }else return 0;	  
} 
//精确读取一次坐标,校准的时候用的	   
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
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
void Drow_Touch_Point(unsigned int x,unsigned int y)
{
	LCD_DrawLine(x-12,y,x+13,y,RED);//横线	12+13=25点长
	LCD_DrawLine(x,y-12,x,y+13,RED);//竖线
	LCD_DrawPoint_big(x+1,y+1,1,BLUE);
	LCD_DrawPoint_big(x-1,y+1,1,BLUE);
	LCD_DrawPoint_big(x+1,y-1,1,BLUE);
	LCD_DrawPoint_big(x-1,y-1,1,BLUE);
//	Draw_Circle(x,y,6,BLUE);//画中心圈
}	  
//转换结果
//根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
//chx,chy为LCD(0,0)处的AD值
//vx,vy为每个像素点占n个AD值
//算法表示采用AD值/n=像素坐标
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
//触摸屏校准代码
//得到四个校准参数
#define tp_pianyi 50   //校准坐标偏移量	
#define tp_xiaozhun 300   //校准精度
void Touch_Adjust(void)
{	
	float vx1,vx2,vy1,vy2;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
	unsigned int chx1,chx2,chy1,chy2;//默认像素点坐标为0时的AD起始值
	TP32_pix xdata p[4];
	unsigned char  cnt=0;
//	unsigned char  showzifu[6];	 
	TFT_ClearByColor(WHITE);//清屏 

	Drow_Touch_Point(tp_pianyi,tp_pianyi);//画校准点1 

	while(1)
	{
		if(TOUCH_IRQ==0)//按键按下了
		{	
			
			if(Read_ADS2(&tp_pixad.x,&tp_pixad.y))//得到单次按键值
			{  								   
				p[cnt].x=tp_pixad.x;
				p[cnt].y=tp_pixad.y;
				cnt++; 
			}
			switch(cnt)
			{			   
				case 1:
					TFT_ClearByColor(WHITE);//清屏 
					while(!TOUCH_IRQ)  //等待松手
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,tp_pianyi);//画点2
					break;
				case 2:
					TFT_ClearByColor(WHITE);//清屏 
					while(!TOUCH_IRQ)  //等待松手
					{
					}
					Drow_Touch_Point(tp_pianyi,LCD_H-tp_pianyi-1);//画点3
					break;
				case 3:
					TFT_ClearByColor(WHITE);//清屏 
					while(!TOUCH_IRQ)  //等待松手
					{
					}
					Drow_Touch_Point(LCD_W-tp_pianyi-1,LCD_H-tp_pianyi-1);//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		   	TFT_ClearByColor(WHITE);//清屏 
				   	while(!TOUCH_IRQ);  //等待松手
					//触摸屏坐标 比 LCD坐标 对应
					//vx1横向一个像素点有多少个AD值
			   		vx1=p[1].x>p[0].x?(p[1].x-p[0].x+1)*1000/(LCD_W-tp_pianyi-tp_pianyi):(p[0].x-p[1].x-1)*1000/(LCD_W-tp_pianyi-tp_pianyi);
				 	//屏初始点（0,0）x坐标对应的AD值
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
						TFT_ClearByColor(WHITE);//清屏 
						Drow_Touch_Point(tp_pianyi,tp_pianyi);//画点1 
						continue;
					}
					vx=(vx1+vx2)/2;vy=(vy1+vy2)/2;
					chx=(chx1+chx2)/2;chy=(chy1+chy2)/2;	
																
					//显示校准信息
					TFT_ClearByColor(WHITE);//清屏 

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
					Read_TP_Once(); //等待任意键后继续
	  */
					TFT_ClearByColor(WHITE);//清屏
					return;//校正完成				 
			}
		}
	} 
}

