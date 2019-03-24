#ifndef __TFT_H
#define __TFT_H	
	
 //����TFT�ĳߴ�,��30*8���ַ�����20*16���ַ�	
#define LCD_W 240
#define LCD_H 320
//������ɫ
#define WHITE         	 0xFFFF	//��ɫ
#define BLACK         	 0x0000	//��ɫ  
#define BLUE         	   0x001F //��ɫ 
#define GBLUE			 			 0X07FF	//����ɫ
#define RED           	 0xF800	//��ɫ
#define MAGENTA       	 0xF81F	//Ʒ���
#define GREEN         	 0x07E0	//��ɫ
#define CYAN          	 0x7FFF	//��ɫ
#define YELLOW        	 0xFFE0	//��ɫ
#define BROWN 			     0XBC40 //��ɫ
#define BRRED 			     0XFC07 //�غ�ɫ
#define GRAY  			     0X8430 //��ɫ
//GUI��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ  
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			     0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

#define BACK_COLOR WHITE
#define POINT_COLOR RED  //����ɫ������ɫ

#define HVSCREEN  //����ñ�����ʾ��������ʾ�������Ǻ�����ʾ
void TFT_Writ_Bus(unsigned char VH,unsigned char VL);
void TFT_WR_DATA8(unsigned char VH,unsigned char VL); 
void TFT_WR_DATA(unsigned int da);
void TFT_WR_REG(unsigned int da);
void TFT_WR_REG_DATA(unsigned int reg,unsigned int da);
void TFT_AddressSet(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void TFT_ClearByColor(unsigned int Color);
void TFT_ClearArea(unsigned int start_x,unsigned int start_y,unsigned int x_size,unsigned int y_size,unsigned int Color);
void TFT_ClearONELINE(unsigned int n,unsigned int Color);
void TFT_Init(void);
/*******************************************��ʾ�ַ����ܺ�����*********************************************************************/ 
//x,yΪ�к��У��������ص�Ӣ��8*16,����16*16
void TFT_ShowWord(unsigned int x,unsigned int y,unsigned int size_x,unsigned int size_y,unsigned char *temp,unsigned int PColor,unsigned int BColor);
void TFT_ShowString(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor);
/*******************************************��ʾͼƬ���ܺ�����*********************************************************************/ 
/*******************************************��ʾ��ͼ���ܺ�����*********************************************************************/ 
void LCD_DrawPoint_big(unsigned int x,unsigned int y,unsigned char n,unsigned int Pen_Color);
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int Pen_Color);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int Pen_Color);
void LCD_DrawBox(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y,unsigned int Pen_Color);
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int Pen_Color);
void TFT_ShowString_w25(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor); 

#endif 
	 
	 



