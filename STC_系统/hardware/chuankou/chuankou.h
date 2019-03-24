#ifndef __CHUANKOU_H
#define __CHUANKOU_H	

#define F_CPU 33177600UL
#define BAUD 9600

#define S2RI 0X01
#define S2TI 0X02

void USART1_Init(void);
void USART2_Init(void);

void SendData1(unsigned char dat);
void SendData2(unsigned char dat);
void SendStr1(unsigned char *s1);
void SendStr2(unsigned char *s2);

void open1(void);
void open2(void);
void open3(void);
void open4(void);
void open5(void);
void open6(void);
void peizhiTouch(void);
void laidian();

#endif 