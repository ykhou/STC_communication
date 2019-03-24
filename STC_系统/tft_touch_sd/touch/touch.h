#ifndef __TOUCH_H
#define __TOUCH_H	
		
/* touch panel interface define */
#define  CMD_RDX  0xD0	 //触摸IC读X坐标寄存器
#define  CMD_RDY  0x90	 //触摸IC读Y坐标寄存器


typedef struct tp_pix_16
{
	unsigned int x;
	unsigned int y;
}TP16_pix;

typedef struct tp_pix_u32
{
	unsigned long x;
	unsigned long y;
}TP32_pix;

extern TP16_pix xdata tp_pixad;
extern TP16_pix xdata tp_pixlcd;	 //当前触控坐标的AD值,前触控坐标的像素值   
extern unsigned int vx,vy;  //比例因子，此值除以1000之后表示多少个AD值代表一个像素点
extern unsigned int chx,chy;//默认像素点坐标为0时的AD起始值

unsigned char Touch_IRQ(void);
void Touch_InitSpi(void); 
unsigned char Convert_Pos(void); 
void Touch_Adjust(void);  

#endif  
	 
	 



