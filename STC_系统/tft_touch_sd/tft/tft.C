#include "STC15F2K60S2.h"
#include "string.h"
#include "font.h"
#include "delay.h"
#include "tft.h"
#include "image.h"
 /*51单片机TFT程序*
 ********************************** ********************
 *51芯片为 STC12 48脚    
 *TFT驱动IC为SSD1289
 ******************************************************
 *TFT接口
	*数据线（输出）： DB0-DB7(低8位),依次连接P2^0-P2^7;
	             DB8-DB15(高8位),依次连接P0^0-P0^7;
	*控制线（输出）：
	P5.2-----------RS
	P5.3-----------WR
	P6.0-----------RD
	P6.4-----------CS
	P6.5-----------REST	

 ******************************************************
 *#define HVSCREEN  定义该变量表示是竖屏显示，否则是横屏显示
 */

#define  TFT_DataPortH P0     //高8位数据口,8位模式下只使用高8位 
#define  TFT_DataPortL P2     //低8位数据口	,8位模式下低8位可以不接线,请确认P0口已经上拉10K电阻,不宜太小，最小4.7K,推荐10K.

sbit TFT_RS = P5^2;  	//数据1/命令0
sbit TFT_WR = P5^3;		//写控制

sbit TFT_RD = P6^0;		//读控制Not used and should be connected to VDDIO or Vss
sbit TFT_CS = P6^4;		//片选，低电平有效	
sbit TFT_REST = P6^5;	//低电平复位 

unsigned char Y_LEN=0;//全局变量用于消息一直往下打印
/***********总线定义*************/   
void TFT_Writ_Bus(unsigned char VH,unsigned char VL)   //并行数据写入函数
{
    TFT_DataPortH=VH;	
	TFT_DataPortL=VL;		
	TFT_WR=0;
	TFT_WR=1; 
}
void TFT_WR_DATA8(unsigned char VH,unsigned char VL)
{
    TFT_RS=1;
	TFT_Writ_Bus(VH,VL);
}
void TFT_WR_DATA(unsigned int da)
{
    TFT_RS=1;
	TFT_Writ_Bus(da>>8,da);
}	  
void TFT_WR_REG(unsigned int da)	 
{	
    TFT_RS=0;
	TFT_Writ_Bus(da>>8,da);
}
 void TFT_WR_REG_DATA(unsigned int reg,unsigned int da)
{
    TFT_WR_REG(reg);
	TFT_WR_DATA(da);
}
/* 函数说明：x1,y1为起始点的坐标；x2,y2为结束点的坐标；
(0,0)x1,y1-------------x2,y1(239,0)
   |					 |
   |			   	     |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   |					 |
   x1,y2-------------x2,y2
  (0,319)		  (239,319)
  函数功能：设置显示区域 显示器竖屏
  R44hRAM地址：水平显示方向的起始(x1低8位)/终点(x2高8位)范围,0x00<=x1<=x2<=0xef(239) 即水平240个点
  R45h-R46hRAM地址：垂直显示方向的起始(y1低8位)/终点(y2低8位)范围,0x00<=y1<=y2<=0x013f(319)	即垂直320个点
  R4eh-R4fhRAM地址：4e为水平地址计数值<=0xef,4f为垂直地址计数值<=0x13f


  函数功能：设置显示区域显示器横屏(X,Y,对调) 
(0,0)x1,y1-------------------------x2,y1(319,0)
   |			               		 |
   |								 |
   |								 |
   |								 |
   |								 |
   x1,y2---------------------------x2,y2
  (0,239)		                  (319,239)

  R44hRAM地址：水平显示方向的起始(y1低8位)/终点(y2高8位)范围,0x00<=y1<=y2<=0xef(239) 即240个点
  R45h-R46hRAM地址：垂直显示方向的起始(x1低8位)/终点(x2低8位)范围,0x00<=x1<=x2<=0x013f(319)即320个点
  R4eh-R4fhRAM地址：4e为水平地址计数值<=0xef,4f为垂直地址计数值<=0x13f
*/
void TFT_AddressSet(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2) 
{ 
   TFT_WR_REG(0x2A);
   TFT_WR_DATA(x1>>8);
   TFT_WR_DATA(x1);
   TFT_WR_DATA(x2>>8);
   TFT_WR_DATA(x2);
  
   TFT_WR_REG(0x2B);
   TFT_WR_DATA(y1>>8);
   TFT_WR_DATA(y1);
   TFT_WR_DATA(y2>>8);
   TFT_WR_DATA(y2);	
   TFT_WR_REG(0x2c);					 	
	
	
//		TFT_WR_REG_DATA(0x0044,(x2<<8)+x1);
//		TFT_WR_REG_DATA(0x0045,y1);
//		TFT_WR_REG_DATA(0x0046,y2);
//		TFT_WR_REG_DATA(0x004e,x1);
//		TFT_WR_REG_DATA(0x004f,y1);	

//	    TFT_WR_REG(0x0022);	 //Write Data to GRAM 
}

void TFT_ClearByColor(unsigned int Color)
{
	unsigned long i; 	 		
	TFT_AddressSet(0,0,239,319); 
    for(i=0;i<76800;i++)
	{
	   TFT_WR_DATA(Color);
	}
}
 /*	清除某个区域*/
void TFT_ClearArea(unsigned int start_x,unsigned int start_y,unsigned int x_size,unsigned int y_size,unsigned int Color)
{
	unsigned long i; 	 		
	TFT_AddressSet(start_x,start_y,start_x+x_size-1,start_y+y_size-1); 
    for(i=0;i<(unsigned long)x_size*y_size;i++)
	{
	   TFT_WR_DATA(Color);
	}
}

//横屏n<240/16=15行，竖屏n<320/16=20行
void TFT_ClearONELINE(unsigned int n,unsigned int Color)
{
	unsigned long i; 	 		
	TFT_AddressSet(0,n*16,239,n*16+15);
	for(i=0;i<16*240;i++)
	{
	   TFT_WR_DATA(Color);
	} 
}

/*
  函数功能：初始化TFT
*/
void TFT_IO_INIT(void)
{
	P0M0=0XFF;
	P0M1=0X00;
	P1M0=0XFF;
	P1M1=0X00;

	P2M0 |=(1<<0)|(1<<1)|(1<<2)|(1<<7);
	P2M1 &=~((1<<0)|(1<<1)|(1<<2)|(1<<7));

	P3M0 |=(1<<7) ;
	P3M1 &=~(1<<7);
}
void TFT_Init(void) 
{
	TFT_IO_INIT();
	TFT_REST=0;delayms(5);//低电平复位
	TFT_REST=1;

	TFT_CS=1;TFT_RD=1;TFT_WR=1;	delayms(5);
	TFT_CS =0;  //打开片选使能

 //************* Start Initial Sequence **********//
TFT_WR_REG(0xcf); 
TFT_WR_DATA(0x00);
TFT_WR_DATA(0xc1);
TFT_WR_DATA(0x30);

TFT_WR_REG(0xed); 
TFT_WR_DATA(0x64);
TFT_WR_DATA(0x03);
TFT_WR_DATA(0x12);
TFT_WR_DATA(0x81);

TFT_WR_REG(0xcb); 
TFT_WR_DATA(0x39);
TFT_WR_DATA(0x2c);
TFT_WR_DATA(0x00);
TFT_WR_DATA(0x34);
TFT_WR_DATA(0x02);

TFT_WR_REG(0xea); 
TFT_WR_DATA(0x00);
TFT_WR_DATA(0x00);

TFT_WR_REG(0xe8); 
TFT_WR_DATA(0x85);
TFT_WR_DATA(0x10);
TFT_WR_DATA(0x79);

TFT_WR_REG(0xC0); //Power control
TFT_WR_DATA(0x23); //VRH[5:0]

TFT_WR_REG(0xC1); //Power control
TFT_WR_DATA(0x11); //SAP[2:0];BT[3:0]

TFT_WR_REG(0xC2);
TFT_WR_DATA(0x11);

TFT_WR_REG(0xC5); //VCM control
TFT_WR_DATA(0x3d);
TFT_WR_DATA(0x30);

TFT_WR_REG(0xc7); 
TFT_WR_DATA(0xaa);

TFT_WR_REG(0x3A); 
TFT_WR_DATA(0x55);

TFT_WR_REG(0x36); // Memory Access Control
TFT_WR_DATA(0x08);

TFT_WR_REG(0xB1); // Frame Rate Control
TFT_WR_DATA(0x00);
TFT_WR_DATA(0x11);

TFT_WR_REG(0xB6); // Display Function Control
TFT_WR_DATA(0x0a);
TFT_WR_DATA(0xa2);

TFT_WR_REG(0xF2); // 3Gamma Function Disable
TFT_WR_DATA(0x00);

TFT_WR_REG(0xF7);
TFT_WR_DATA(0x20);

TFT_WR_REG(0xF1);
TFT_WR_DATA(0x01);
TFT_WR_DATA(0x30);

TFT_WR_REG(0x26); //Gamma curve selected
TFT_WR_DATA(0x01);

TFT_WR_REG(0xE0); //Set Gamma
TFT_WR_DATA(0x0f);
TFT_WR_DATA(0x3f);
TFT_WR_DATA(0x2f);
TFT_WR_DATA(0x0c);
TFT_WR_DATA(0x10);
TFT_WR_DATA(0x0a);
TFT_WR_DATA(0x53);
TFT_WR_DATA(0xd5);
TFT_WR_DATA(0x40);
TFT_WR_DATA(0x0a);
TFT_WR_DATA(0x13);
TFT_WR_DATA(0x03);
TFT_WR_DATA(0x08);
TFT_WR_DATA(0x03);
TFT_WR_DATA(0x00);

TFT_WR_REG(0xE1); //Set Gamma
TFT_WR_DATA(0x00);
TFT_WR_DATA(0x00);
TFT_WR_DATA(0x10);
TFT_WR_DATA(0x03);
TFT_WR_DATA(0x0f);
TFT_WR_DATA(0x05);
TFT_WR_DATA(0x2c);
TFT_WR_DATA(0xa2);
TFT_WR_DATA(0x3f);
TFT_WR_DATA(0x05);
TFT_WR_DATA(0x0e);
TFT_WR_DATA(0x0c);
TFT_WR_DATA(0x37);
TFT_WR_DATA(0x3c);
TFT_WR_DATA(0x0F);
TFT_WR_REG(0x11); //Exit Sleep
delayms(80);//此处必须要延时80ms或者更多
TFT_WR_REG(0x29); //display on
	
	
// 	TFT_WR_REG_DATA(0x0000,0x0001);      //打开晶振
//    TFT_WR_REG_DATA(0x0003,0xA8A4);      //0xA8A4
//    TFT_WR_REG_DATA(0x000C,0x0000);       
//    TFT_WR_REG_DATA(0x000D,0x080C);      
//    TFT_WR_REG_DATA(0x000E,0x2B00);     
//    TFT_WR_REG_DATA(0x001E,0x00B7);       
//    TFT_WR_REG_DATA(0x0001,0x2B3F);      //驱动输出控制320*240  0x6B3F

//    TFT_WR_REG_DATA(0x0002,0x0600);   
//    TFT_WR_REG_DATA(0x0010,0x0000);  

//#ifdef HVSCREEN  	  	//竖屏  16位色 
////	 TFT_WR_REG_DATA(0x0011,0x6838);  //全屏显示320*240视频时配置（此时汉字不能显示）,自上而下,从左到右
//    TFT_WR_REG_DATA(0x0011,0x6830);    //显示字符、汉字、视频（宽度小于240）时配置,从左到右 ,自上而下   
//#else				    //宽屏	16位色 
// 	 TFT_WR_REG_DATA(0x0011,0x6818);  //目前显示字符、汉字、视频都可以,自下而上,从左到右 
//#endif
//	      
//    TFT_WR_REG_DATA(0x0005,0x0000);    
//    TFT_WR_REG_DATA(0x0006,0x0000);    
//    TFT_WR_REG_DATA(0x0016,0xEF1C);    
//    TFT_WR_REG_DATA(0x0017,0x0003);    
//    TFT_WR_REG_DATA(0x0007,0x0233);            //0x0233       
//    TFT_WR_REG_DATA(0x000B,0x0000);    
//    TFT_WR_REG_DATA(0x000F,0x0000);            //扫描开始地址
//    TFT_WR_REG_DATA(0x0041,0x0000);    
//    TFT_WR_REG_DATA(0x0042,0x0000);    
//    TFT_WR_REG_DATA(0x0048,0x0000);    
//    TFT_WR_REG_DATA(0x0049,0x013F);    
//    TFT_WR_REG_DATA(0x004A,0x0000);    
//    TFT_WR_REG_DATA(0x004B,0x0000);    
//    TFT_WR_REG_DATA(0x0044,0xEF00);    
//    TFT_WR_REG_DATA(0x0045,0x0000);    
//    TFT_WR_REG_DATA(0x0046,0x013F);    
//    TFT_WR_REG_DATA(0x0030,0x0707);    
//    TFT_WR_REG_DATA(0x0031,0x0204);    
//    TFT_WR_REG_DATA(0x0032,0x0204);    
//    TFT_WR_REG_DATA(0x0033,0x0502);    
//    TFT_WR_REG_DATA(0x0034,0x0507);    
//    TFT_WR_REG_DATA(0x0035,0x0204);    
//    TFT_WR_REG_DATA(0x0036,0x0204);    
//    TFT_WR_REG_DATA(0x0037,0x0502);    
//    TFT_WR_REG_DATA(0x003A,0x0302);    
//    TFT_WR_REG_DATA(0x003B,0x0302);    
//    TFT_WR_REG_DATA(0x0023,0x0000);    
//    TFT_WR_REG_DATA(0x0024,0x0000);    
//    TFT_WR_REG_DATA(0x0025,0x8000);    
	TFT_ClearByColor(BACK_COLOR);
}
/*******************************************显示字符功能函数区*********************************************************************/
/*
  函数功能：指定位置显示任意大小的字符(可以是汉字) 默认字符为8*16，汉字为16*16
  x,y:起始字符的行列，x<30列宽为8的字符,y<20行高为16的字符
  size_x,size_y:字体的大小
  PColor为内容颜色，BColor为背静颜色
*/
void TFT_ShowWord(unsigned int x,unsigned int y,unsigned int size_x,unsigned int size_y,unsigned char *temp,unsigned int PColor,unsigned int BColor)	
{  
	unsigned char i,j;
	TFT_AddressSet(x,y,x+size_x-1,y+size_y-1); //设置区域 
	
	for(j=0;j<size_x*size_y/8;j++)	  //如果字符为16个数组，如果为汉字为32个数组
	{
		for(i=8;i>0;i--)	    //高位在前，低位在后，取字模时匹配就OK
		{ 		     
		 	if((*temp&(1<<(i-1)))>0)  //最高位为左移7位
			{
				TFT_WR_DATA(PColor);  //对应于取字模时的阴码
			} 
			else
			{
				TFT_WR_DATA(BColor);
			}   
		}
		temp++;
	 }
}
/*
  函数功能：指定位置显示显示字符串	默认字符为8*16，汉字为16*16
  x,y:起始字符的行列:竖屏x<30列(240/8) y<20行(320/16);宽屏x<30列(320/8=40) y<15行(240/16)
  *ptr:显示内容
  PenColor为内容颜色，BackColor为背静颜色
*/
void TFT_ShowString(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor)
{         
 	unsigned char i,j,val=0; //一定要初始化，不然xlen在第二次调用时会有问题

	unsigned char temp[2];
	unsigned char xlen;
	unsigned char ylen;	
    unsigned char max_ylen;unsigned char max_xlen;
#ifdef HVSCREEN  	  	//竖屏   
    max_xlen=30;max_ylen=20;      	//最多显示240/8=30列 320/16=20行 
#else				    //宽屏	
    max_xlen=40;max_ylen=15;      	//最多显示320/8=40列 240/16=15行     		
#endif
	for(i=0;i<strlen(ptr);i++)
	{
	 	temp[0]=ptr[i];
		temp[1]=ptr[i+1];
		
		xlen =(x+i+val)%max_xlen;	   	 		
		ylen =((x+i+val)/max_xlen+y)%max_ylen;  
//		if(xlen==0 && ylen ==0)
//			TFT_ClearByColor(BACK_COLOR);

		if(temp[0]<128)		//为ASCII码
			TFT_ShowWord(xlen*8,ylen*16,8,16,ASCII_816[temp[0]-0x20],PenColor,BackColor); //ASCII码前32个字库未录入temp[0]-0x20)，每个字符16个
		else 			   //为汉字
	   	{
			if(xlen==max_xlen-1)   //最后一列不能放下一个汉字
			{
				TFT_ShowWord(xlen*8,ylen*16,8,16,ASCII_816[0],PenColor,BackColor);  //显示“ ”空白填补
				val++;	
				i--;//指针还是指向原汉字,所以要减一			
			}
			else
			{
				for(j=0;j<sizeof(GB16)/35;j++)	 //查找汉字库，34字节一个字符
				{
					if((temp[0]==GB16[j].Index[0])&&(temp[1]==GB16[j].Index[1]))//找到需要的汉字字节
					{
						break;
					}
				}
				if(j<sizeof(GB16)/35) 				
				TFT_ShowWord(xlen*8,ylen*16,16,16,&GB16[j].MSK,PenColor,BackColor); 
				i++;
			}
		 }
	}
	Y_LEN = ylen+1; //记录下用到的行数+下一行
	Y_LEN %=max_ylen;
}

/*******************************************显示图片功能函数区*********************************************************************/ 

//n为像素大小>0
//POINT_COLOR:此点的颜色	 
void LCD_DrawPoint_big(unsigned int x,unsigned int y,unsigned char n,unsigned int Pen_Color)
{
	unsigned char i;
	if(n>0)
	{
		TFT_AddressSet(x,y,x+n-1,y+n-1);//设置光标位置 
		for(i=0;i<n*n;i++)
		TFT_WR_DATA(Pen_Color); 
	}
}
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int Pen_Color)
{
	unsigned int t; 
	int xerr=0,yerr=0,dx,dy,distance; 
	int incx,incy,uRow,uCol; 

	dx=x2-x1; //计算坐标增量 
	dy=y2-y1; 
	
	uRow=x1; 
	uCol=y1; 

	if(dx>0)incx=1; //设置单步方向 
	else if(dy==0)incx=0;//垂直线 
	else {incx=-1;dx=-dx;} 

	if(dy>0)incy=1; 
	else if(dy==0)incy=0;//水平线 
	else{incy=-1;dy=-dy;} 

	if(dx>dy) distance=dx; //选取基本增量坐标轴 
	else distance=dy; 

	for(t=0;t<=distance;t++ )//画线输出 
	{  
		LCD_DrawPoint_big(uRow,uCol,1,Pen_Color);//画点 
		xerr+=dx; 
		yerr+=dy; 
		if(xerr>=distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>=distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 
 
//画矩形
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int Pen_Color)
{
	LCD_DrawLine(x1,y1,x2,y1,Pen_Color); //上横
	LCD_DrawLine(x1,y1,x1,y2,Pen_Color); //左竖
	LCD_DrawLine(x1,y2,x2,y2,Pen_Color); //下横
	LCD_DrawLine(x2,y1,x2,y2,Pen_Color); //右竖
}
/*********************************************
画实心矩形
入口参数： (x1,y1)左上角顶点坐标, (x2,y2)右下角顶点坐标, Pen_Color颜色
出口参数: 无
说明：在指定位置上画出实心矩形。
**********************************************/
void LCD_DrawBox(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y,unsigned int Pen_Color)
{ 
	unsigned int i;
	TFT_AddressSet(x,y,x+size_x-1,y+size_y-1);//设置光标位置 
	for(i=0;i<size_x*size_y;i++)
		TFT_WR_DATA(Pen_Color); 
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int Pen_Color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //判断下个点位置的标志
	while(a<=b)
	{
		LCD_DrawPoint_big(x0-b,y0-a,1,Pen_Color);             //3           
		LCD_DrawPoint_big(x0+b,y0-a,1,Pen_Color);             //0           
		LCD_DrawPoint_big(x0-a,y0+b,1,Pen_Color);             //1       
		LCD_DrawPoint_big(x0-b,y0-a,1,Pen_Color);             //7           
		LCD_DrawPoint_big(x0-a,y0-b,1,Pen_Color);             //2             
		LCD_DrawPoint_big(x0+b,y0+a,1,Pen_Color);             //4               
		LCD_DrawPoint_big(x0+a,y0-b,1,Pen_Color);             //5
		LCD_DrawPoint_big(x0+a,y0+b,1,Pen_Color);             //6 
		LCD_DrawPoint_big(x0-b,y0+a,1,Pen_Color);             
		a++;
		//使用Bresenham算法画圆     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint_big(x0+a,y0+b,1,Pen_Color);
	}
} 









