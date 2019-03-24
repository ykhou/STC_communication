#ifndef __TFT_H
#define __TFT_H	
	
 //定义TFT的尺寸,行30*8个字符，列20*16个字符	
#define LCD_W 240
#define LCD_H 320
//画笔颜色
#define WHITE         	 0xFFFF	//白色
#define BLACK         	 0x0000	//黑色  
#define BLUE         	   0x001F //蓝色 
#define GBLUE			 			 0X07FF	//蓝绿色
#define RED           	 0xF800	//红色
#define MAGENTA       	 0xF81F	//品红粉
#define GREEN         	 0x07E0	//绿色
#define CYAN          	 0x7FFF	//青色
#define YELLOW        	 0xFFE0	//黄色
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色  
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#define BACK_COLOR WHITE
#define POINT_COLOR RED  //背景色，画笔色

#define HVSCREEN  //定义该变量表示是竖屏显示，否则是横屏显示
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
/*******************************************显示字符功能函数区*********************************************************************/ 
//x,y为行和列，不是像素点英文8*16,汉字16*16
void TFT_ShowWord(unsigned int x,unsigned int y,unsigned int size_x,unsigned int size_y,unsigned char *temp,unsigned int PColor,unsigned int BColor);
void TFT_ShowString(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor);
/*******************************************显示图片功能函数区*********************************************************************/ 
/*******************************************显示绘图功能函数区*********************************************************************/ 
void LCD_DrawPoint_big(unsigned int x,unsigned int y,unsigned char n,unsigned int Pen_Color);
void LCD_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int Pen_Color);
void LCD_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int Pen_Color);
void LCD_DrawBox(unsigned int x, unsigned int y, unsigned int size_x, unsigned int size_y,unsigned int Pen_Color);
void Draw_Circle(unsigned int x0,unsigned int y0,unsigned int r,unsigned int Pen_Color);
void TFT_ShowString_w25(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor); 

#endif 
	 
	 



