#ifndef __IIC_H
#define __IIC_H	

void Init_IIC(void);
void I2CStart(void);
void I2CStop(void);
void I2CSendACK(bit ack);//入口参数:ack (0:ACK 1:NAK)
bit I2CRecvACK(void);
void I2CSendByte(unsigned char dat);
unsigned char  I2CRecvByte(void);


#endif 