#include "STC15F2K60S2.h"
#include "tft.h"
#include "txt.h"
#include "iic.h"
#include "delay.h"
#include "qiya_bmp085.h"
#define	BMP085_AD   0xee	  //气压BMP085

//?????BMP085????*******************************

#define	BMP085_AD   0xee	  //气压BMP085
#define OSS 0	
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

unsigned char xdata bmp085BUF[7];   
void Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)
{
    I2CStart();                  //????
    I2CSendByte(SlaveAddress);   //??????+???
    I2CSendByte(REG_Address);    //???????
    I2CSendByte(REG_data);       //???????
    I2CStop();                   //??????
}
unsigned char Single_Read(unsigned char REG_Address)
{  unsigned char REG_data;
    I2CStart();                          //????
    I2CSendByte(BMP085_AD);           //??????+???
    I2CSendByte(REG_Address);            //????????	
    I2CStart();                          //????
    I2CSendByte(BMP085_AD+1);         //??????+???
    REG_data=I2CRecvByte();              //???????
		I2CSendACK(1);   
		I2CStop();                           //????
    return REG_data; 
}
int Multiple_read(unsigned char ST_Address)
{   
		unsigned char msb, lsb;
		int _data;
    I2CStart();                          //????
    I2CSendByte(BMP085_AD);    //??????+???
    I2CSendByte(ST_Address);             //????????
    I2CStart();                          //????
    I2CSendByte(BMP085_AD+1);         //??????+???

    msb = I2CRecvByte();                 //BUF[0]??
    I2CSendACK(0);                       //??ACK
    lsb = I2CRecvByte();     
		I2CSendACK(1);                       //?????????NOACK

    I2CStop();                           //????
    delayms(5);
    _data = msb << 8;
		_data |= lsb;	
		return _data;
}
//***************读温度*****************************************************
int bmp085ReadTemp(void)
{

    I2CStart();                  //????
    I2CSendByte(BMP085_AD);   //??????+???
    I2CSendByte(0xF4);	          // write register address
    I2CSendByte(0x2E);       	// write register data for temp
    I2CStop();                   //??????
		delay(10);	// max time is 4.5ms
	
	return Multiple_read(0xF6);
}
//****************读压强*********************************************
int bmp085ReadPressure(void)
{
		I2CStart();                   //????
    I2CSendByte(BMP085_AD);   //??????+???
    I2CSendByte(0xF4);	          // write register address
    I2CSendByte(0x34);       	  // write register data for pressure
    I2CStop();                    //??????
		delay(10);    	                  // max time is 4.5ms
	
		return Multiple_read(0xF6);	
}

//**************************************************************
void Init_BMP085(void)
{
	ac1 = Multiple_read(0xAA);
	ac2 = Multiple_read(0xAC);
	ac3 = Multiple_read(0xAE);
	ac4 = Multiple_read(0xB0);
	ac5 = Multiple_read(0xB2);
	ac6 = Multiple_read(0xB4);
	b1 =  Multiple_read(0xB6);
	b2 =  Multiple_read(0xB8);
	mb =  Multiple_read(0xBA);
	mc =  Multiple_read(0xBC);
	md =  Multiple_read(0xBE);
}
 
void conversion(long temp_data)  
{  
    bmp085BUF[0]=temp_data/100000+0x30 ;
    temp_data=temp_data%100000;   //???? 
    bmp085BUF[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	  bmp085BUF[2]=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //????
    bmp085BUF[3]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    bmp085BUF[4]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    bmp085BUF[5]=temp_data+0x30; 	
		bmp085BUF[6]='\0';
}
void bmp085Convert(void)
{
	long ut;
	long up;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;
	long  temperature;
	long  pressure;
	
	ut = bmp085ReadTemp();
	ut = bmp085ReadTemp();	   // ????
	up = bmp085ReadPressure();
	up = bmp085ReadPressure();  // ????
	
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	temperature = (b5 + 8) >> 4;
	
  conversion(temperature);
	TFT_ShowString_W25(0,2,2,"温度:",WHITE,BLUE);
	TFT_ShowString(5,2,bmp085BUF,WHITE,BLUE);
  TFT_ShowString_W25(11,2,2,"度",WHITE,BLUE); 
	
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((long)ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> OSS);
	if( b7 < 0x80000000)
	     p = (b7 * 2) / b4 ;
           else  
		    p = (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	 pressure = p + ((x1 + x2 + 3791) >> 4);	
	conversion(pressure);
	TFT_ShowString_W25(15,2,2,"气压:",WHITE,BLUE);
	TFT_ShowString(20,2,bmp085BUF,WHITE,BLUE);
  TFT_ShowString_W25(26,2,2,"帕",WHITE,BLUE); 					 
					 
}

