#ifndef __BIN_H
#define __BIN_H
void DISP_BIN(unsigned char cjd);
void DISP_BMP(unsigned char cjd);
void DISP_BIN_PT_W25(unsigned int x1,unsigned int y1,unsigned int width,unsigned int height,unsigned long tReadAddr1,unsigned long tReadAddr2);
void DISP_BIN_PT(unsigned int x1,unsigned int y1,unsigned int width,unsigned int height,unsigned char *tmp_Char_Model);
#endif


