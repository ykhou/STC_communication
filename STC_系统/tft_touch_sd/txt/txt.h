#ifndef __TXT_H
#define __TXT_H

void TFT_ShowWord_SD(unsigned int x, unsigned int  y, unsigned char *Disp_char, unsigned int PenColor,unsigned int BackColor);
void TFT_ShowString_SD(unsigned int x,unsigned int y,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor);
void DISP_TXT(unsigned char cjd);
void TFT_ShowWord_W25(unsigned int x, unsigned int  y, unsigned int size_x,unsigned int size_y,unsigned char *temp,unsigned int PColor,unsigned int BColor);
void TFT_ShowString_W25(unsigned int x,unsigned int y,unsigned int size,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor);
#endif


