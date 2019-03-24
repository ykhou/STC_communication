#include "STC15F2K60S2.h"
#include "spi.h"

/**单片机作为主机，NRF24L01作为从机**/
sbit NR_MOSI = P4^1;
sbit NR_MISO = P4^2;
sbit NR_SCK = P4^3;

void Init_SPI(void)		     //SPI初始化，
{
	AUXR1 |= 0X20;	//将 SPI 调整到 P4.1 P4.2 P4.3,默认是在P1口
	SPDAT = 0;		//清空SPI寄存器
	SPCTL = 0xd0;	//1101 0000 
					//SSIG 1 开启主机模式 SPEN 1 SPI使能  DORD 0 先发最高位   MSTR 1 主机模式
					// CPOL 0  SPICLK空闲时为低 
					// CPHA  0  在ssig=1时未定义(数据在SPICLK的前时钟沿驱动)  时钟CPU_CLK/4 
	SPSTAT=0xc0;             //SPDAT.7和SPDAT.6写11，可以将中断标志清零。注意是写1才清零
	P4M1 |=(1<<2)|(1<<5);
	P4M0 &=~(1<<2)|(1<<5);
	//IE2 |= 0x02;           //允许SPI中断
	//EA =1;                 //开总中断
}
//SPI为全双工通讯  所以在发送的同时可以接收到数据
unsigned char SPI_SendByte(unsigned char SPI_SendData)
{
	unsigned char i;
   	for(i=0;i<8;i++)              // output 8-bit
   	{
		NR_MOSI = (SPI_SendData & 0x80);         // output 'uchar', MSB to MOSI
		SPI_SendData = (SPI_SendData << 1);           // shift next bit into MSB..
		NR_SCK = 1;                      // Set SCK high..
		SPI_SendData |= NR_MISO;//放在下语句后也行 // capture current MISO bit
		NR_SCK = 0;            		  // ..then set SCK low again
   	}

    return(SPI_SendData);           		  // return read uchar
/*	SPDAT= SPI_SendData;     // 将数据 写入 SPDAT数据寄存器
	while((SPSTAT&0x80)==0); //等待写入完成
	SPSTAT = 0xc0;           //清除中断标志，和写冲突标志，注意是对应位写1才能清零
	return  SPDAT;           //返回得到的数据
*/
}


/****************************************************************************************************
/*函数：uint SD_SPI_SendByte(uint uchar)
/*功能：IO模拟SPI主机通信时序
 SPI：上升沿发送、下降沿接收、高位先发送。 
     上升沿到来的时候，sdo上的电平将被发送到从设备的寄存器中。
	 下降沿到来的时候，sdi上的电平将被接收到主设备的寄存器中。
/***************************************************************************************************
unsigned char NRF_SPI_SendByte(unsigned char uchar)
{
	unsigned char i;
   	for(i=0;i<8;i++) // output 8-bit
   	{
		SD_MOSI = (uchar & 0x80);         // output 'uchar', MSB to MOSI
		uchar = (uchar << 1);           // shift next bit into MSB..
		SD_SCK = 1;                      // Set SCK high..
		uchar |= SD_MISO;//放在下语句后也行 // capture current MISO bit
		SD_SCK = 0;            		  // ..then set SCK low again
   	}
    return(uchar);           		  // return read uchar
}


*/



