#ifndef __TOUCH_H
#define __TOUCH_H	
		
/* touch panel interface define */
#define  CMD_RDX  0xD0	 //����IC��X����Ĵ���
#define  CMD_RDY  0x90	 //����IC��Y����Ĵ���


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
extern TP16_pix xdata tp_pixlcd;	 //��ǰ���������ADֵ,ǰ�������������ֵ   
extern unsigned int vx,vy;  //�������ӣ���ֵ����1000֮���ʾ���ٸ�ADֵ����һ�����ص�
extern unsigned int chx,chy;//Ĭ�����ص�����Ϊ0ʱ��AD��ʼֵ

unsigned char Touch_IRQ(void);
void Touch_InitSpi(void); 
unsigned char Convert_Pos(void); 
void Touch_Adjust(void);  

#endif  
	 
	 



