#include "STC15F2K60S2.h"
#include "delay.h"
#include "iic.h"
/**
  iic.c功能
*/
sbit I2C_SCL =P4^6;
sbit I2C_SDA =P4^5;

void Init_IIC(void)
{
P4M0 |=(1<<6);	  //I2C_SCL
P4M1 &=~(1<<6);	

P4M0 |=(1<<5);	  //I2C_SDA 
P4M1 &=~(1<<5);	

}

/**************************************
起始信号:SCL为高电平时，sda有个下降沿
**************************************/
void I2CStart(void)
{
    I2C_SDA = 1;                    //拉高数据线
    I2C_SCL = 1;                    //拉高时钟线
    delay(10);                 //延时
    I2C_SDA = 0;                    //产生下降沿
    delay(10);                 //延时
    I2C_SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号:SCL为高电平时，sda有个上升沿
**************************************/
void I2CStop(void)
{
    I2C_SDA = 0;                    //拉低数据线
    I2C_SCL = 1;                    //拉高时钟线
    delay(10);                 //延时
    I2C_SDA = 1;                    //产生上升沿
    delay(10);                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void I2CSendACK(bit ack)
{
    I2C_SDA = ack;                  //写应答信号
    I2C_SCL = 1;                    //拉高时钟线
    delay(10);                 //延时
    I2C_SCL = 0;                    //拉低时钟线
    delay(10);                 //延时
}

/**************************************
接收应答信号
**************************************/
bit I2CRecvACK(void)
{
    I2C_SCL = 1;                    //拉高时钟线

	P4M0 &=~(1<<5);	      //I2C_SDA  配置为输入
    P4M1 |=(1<<5);

	delay(10);                 //延时
    CY = I2C_SDA;                   //读应答信号
    I2C_SCL = 0;                    //拉低时钟线
    
	P4M0 |=(1<<5);	   //I2C_SDA   配置为输出
	P4M1 &=~(1<<5);	
	
	delay(10);                 //延时

    return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void I2CSendByte(unsigned char dat)
{
    unsigned char  i;

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;              //移出数据的最高位
        I2C_SDA = CY;               //送数据口
        I2C_SCL = 1;                //拉高时钟线
        delay(10);             //延时
        I2C_SCL = 0;                //拉低时钟线
        delay(10);             //延时
    }
    I2CRecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
unsigned char  I2CRecvByte(void)
{
    unsigned char  i;
    unsigned char  dat = 0;

	P4M0 &=~(1<<5);	      //I2C_SDA  配置为输入
    P4M1 |=(1<<5);

    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        I2C_SCL = 1;                //拉高时钟线
        delay(10);             //延时
        dat |= I2C_SDA;             //读数据               
        I2C_SCL = 0;                //拉低时钟线
        delay(20);             //延时
    }

	P4M0 |=(1<<5);	   //I2C_SDA   配置为输出
	P4M1 &=~(1<<5);	
    return dat;
}