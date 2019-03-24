#include "STC15F2K60S2.h"
#include "spi.h"

/**��Ƭ����Ϊ������NRF24L01��Ϊ�ӻ�**/
sbit NR_MOSI = P4^1;
sbit NR_MISO = P4^2;
sbit NR_SCK = P4^3;

void Init_SPI(void)		     //SPI��ʼ����
{
	AUXR1 |= 0X20;	//�� SPI ������ P4.1 P4.2 P4.3,Ĭ������P1��
	SPDAT = 0;		//���SPI�Ĵ���
	SPCTL = 0xd0;	//1101 0000 
					//SSIG 1 ��������ģʽ SPEN 1 SPIʹ��  DORD 0 �ȷ����λ   MSTR 1 ����ģʽ
					// CPOL 0  SPICLK����ʱΪ�� 
					// CPHA  0  ��ssig=1ʱδ����(������SPICLK��ǰʱ��������)  ʱ��CPU_CLK/4 
	SPSTAT=0xc0;             //SPDAT.7��SPDAT.6д11�����Խ��жϱ�־���㡣ע����д1������
	P4M1 |=(1<<2)|(1<<5);
	P4M0 &=~(1<<2)|(1<<5);
	//IE2 |= 0x02;           //����SPI�ж�
	//EA =1;                 //�����ж�
}
//SPIΪȫ˫��ͨѶ  �����ڷ��͵�ͬʱ���Խ��յ�����
unsigned char SPI_SendByte(unsigned char SPI_SendData)
{
	unsigned char i;
   	for(i=0;i<8;i++)              // output 8-bit
   	{
		NR_MOSI = (SPI_SendData & 0x80);         // output 'uchar', MSB to MOSI
		SPI_SendData = (SPI_SendData << 1);           // shift next bit into MSB..
		NR_SCK = 1;                      // Set SCK high..
		SPI_SendData |= NR_MISO;//����������Ҳ�� // capture current MISO bit
		NR_SCK = 0;            		  // ..then set SCK low again
   	}

    return(SPI_SendData);           		  // return read uchar
/*	SPDAT= SPI_SendData;     // ������ д�� SPDAT���ݼĴ���
	while((SPSTAT&0x80)==0); //�ȴ�д�����
	SPSTAT = 0xc0;           //����жϱ�־����д��ͻ��־��ע���Ƕ�Ӧλд1��������
	return  SPDAT;           //���صõ�������
*/
}


/****************************************************************************************************
/*������uint SD_SPI_SendByte(uint uchar)
/*���ܣ�IOģ��SPI����ͨ��ʱ��
 SPI�������ط��͡��½��ؽ��ա���λ�ȷ��͡� 
     �����ص�����ʱ��sdo�ϵĵ�ƽ�������͵����豸�ļĴ����С�
	 �½��ص�����ʱ��sdi�ϵĵ�ƽ�������յ����豸�ļĴ����С�
/***************************************************************************************************
unsigned char NRF_SPI_SendByte(unsigned char uchar)
{
	unsigned char i;
   	for(i=0;i<8;i++) // output 8-bit
   	{
		SD_MOSI = (uchar & 0x80);         // output 'uchar', MSB to MOSI
		uchar = (uchar << 1);           // shift next bit into MSB..
		SD_SCK = 1;                      // Set SCK high..
		uchar |= SD_MISO;//����������Ҳ�� // capture current MISO bit
		SD_SCK = 0;            		  // ..then set SCK low again
   	}
    return(uchar);           		  // return read uchar
}


*/



