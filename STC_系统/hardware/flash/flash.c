#include "STC15F2K60S2.h"
#include <intrins.h>
#include  "delay.h"				  //��ʱ����
#include  "tft.h"
#include  "flash.h"
#include  "stdio.h"
#include  "chuankou.h"
/**��Ƭ����Ϊ������W25Q64��Ϊ�ӻ�**/
sbit W25_CS = P5^0;
sbit NR_MOSI = P1^3;
sbit NR_MISO = P1^4;
sbit NR_SCK = P1^5;

sbit SD_CS   = P1^2;

unsigned int DEVICE_ID = 0xEF17;  //W25Q64�������̱�־
bit f_spistatus = 0;




/************************************************
SPI�жϷ������
//***********************************************/
//void spi_isr() interrupt 9
//{
//	SPSTAT = SPIF|WCOL;  //���SPI״̬λ
//	f_spistatus = FALSE;
//}

/****************************************
SPI��ʼ��		 ��ʼ��ʱδʹ��SPI
*****************************************/
void InitSpi(void)
{
    P1M1 |=(1<<4);
	P1M0 &=~(1<<4);

	P1M1 &=~(1<<3)|(1<<5);
	P1M0 |=(1<<3)|(1<<5);

	P5M1 &=~(1<<0);
	P5M0 |=(1<<0);

	ACC = P_SW1;				 //�л�����һ��spi
	ACC &= ~(SPI_S0|SPI_S1);	 //SPI_S0=0;SPI_S1=0
	P_SW1 |= ACC;				 //(P1.2/SS,P1.3/MOSI,P1.4/MISO,P1.5/SCLK)

	SPSTAT = SPIF|WCOL;          //���SPI״̬
	W25_CS = 1;
	SPDAT = 0;		//���SPI�Ĵ���
	SPCTL = SPEN|SSIG|MSTR|SPDL;      //����SPIΪ��ģʽ,
									//ע�⣺ʱ��ѡΪCPU_CLK/16,ѡ��CPU_CLK/4�����������ʱ��Ƶ��һ�㲻Ҫ����3MHz

//	IE2|=ESPI;
//	EA =1;
}
// ���ܣ���ȡW25Q16оƬ��״̬��
// ����ֵ��״̬�Ĵ��������ֽ�
// ע��W25X16�ڲ�״̬�Ĵ�����0λ=0��ʾ���У�0λ=1��ʾæ��
unsigned char W25Q16_ReadStatus()
{
    unsigned char status=0;
    W25_CS=0;
    SpiSendData(W25X_CMD_ReadStatusReg1);   // 0x05��ȡ״̬��������
    status=SpiSendData(0xff);        // ��ȡ״̬�ֽ�
    W25_CS=1;                         // �ر�Ƭѡ
    return status;
} 
/**********************************************************************
�������ƣ�ReadDeviceID
�������ܣ���ȡDevice ID
��ڲ�������
���ڲ�����
            DeviceID-------->�豸ID
***********************************************************************/
unsigned int ReadDeviceID(void)
{
    unsigned int DeviceID=0;
    while(W25Q16_ReadStatus()&0x01);    //�ж��Ƿ�æ
    W25_CS = 0;
//	f_spistatus=1;
    SpiSendData(0x90);
    
    SpiSendData(0x00);
    SpiSendData(0x00);
    SpiSendData(0x00);
	
    DeviceID = (SpiSendData(0xff) << 8);
    DeviceID |= SpiSendData(0xff);
    W25_CS = 1;
    return DeviceID;
}
/***********************************************************************
�������ƣ�SpiSendData
�������ܣ�SPI��������
��ڲ�����
          tData----------------->����������
���ڲ�������
************************************************************************/

unsigned char SpiSendData(unsigned char tData)
{  
    SPDAT = tData;			//���ͻ�����Ϊ��ʱ��������  
    while(!(SPSTAT & 0x80));//�ȴ��������
	SPSTAT = SPIF|WCOL;//����״̬�Ĵ�����ֵ
	return SPDAT;
}

void Spi_Flash_init(void)
{
//	DEVICE_ID = ReadDeviceID();//��ȡFlash ID
	Flash_ChipErase();//Flash ����
}

/**********************************************************************
�������ƣ�Flash_32K_Erase
�������ܣ�32K �������
��ڲ�����
���ڲ�����
***********************************************************************/
void Flash_64K_Erase(unsigned long EraseAddr)
{

//	EraseAddr*=W25Q64_BLOCK_SIZE;
	SpiWriteEn();
	
	W25_CS = 0;
	
	SpiSendData(0xd8);  //64K�������
	SpiSendData((EraseAddr & 0xFF0000) >> 16);
	SpiSendData((EraseAddr & 0xFF00) >> 8);
	SpiSendData(EraseAddr & 0xFF);
	W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}  
/**********************************************************************
�������ƣ�Flash_4K_Erase
�������ܣ�4K �������  ��������
��ڲ�����
���ڲ�����
***********************************************************************/
void Flash_4K_Erase(unsigned long EraseAddr)
{

//    EraseAddr*=W25Q64_SECTOR_SIZE;
    SpiWriteEn();
    
    W25_CS = 0;
    
    SpiSendData(0x20);  //4K�������
    SpiSendData((EraseAddr & 0xFF0000) >> 16);
    SpiSendData((EraseAddr & 0xFF00) >> 8);
    SpiSendData(EraseAddr & 0xFF);
    W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************
�������ƣ�Flash_ChipErase
�������ܣ���Ƭ����                 					  
*********************************************************************/                      
void Flash_ChipErase(void)
{
	SpiWriteEn(); //дʹ��
	W25_CS = 0;
//	f_spistatus=2;
	SpiSendData(W25X_CMD_ChipErase);   //������Ƭ����ָ��
	W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}
/***********************************************************************
�������ܣ�SPI��ʱ
***********************************************************************/
void SpiDelay(void)
{
    _nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}
/***********************************************************************
�������ƣ�SpiWriteEn
�������ܣ�SPIдʹ��
��ڲ�������
���ڲ�������
************************************************************************/
void SpiWriteEn(void)
{
    W25_CS = 0;
//  SpiDelay();
//	f_spistatus=3;
    SpiSendData(W25X_CMD_WriteEnable);	 //����дʹ��ָ��
    W25_CS = 1;
}   
/**************************************************************************
�������ƣ�SPI_FLASH_WaitForWriteEnd
�������ܣ���FlashоƬ״̬�Ĵ�����ֵ����ѭ����ȡ��־λֱ��д���ڽ���
��ڲ�������
���ڲ�������
 *************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;
  W25_CS = 0;

  // ���Ͷ�״̬�Ĵ������������֮��״̬�Ĵ�����ֵ�ᱻ���͵�STM8 
//  f_spistatus=4;
  SpiSendData(W25X_CMD_ReadStatusReg1); //��ȡFlash ״̬�Ĵ���1

  // ѭ����ѯ��־λ �ȴ�д���ڽ��� 
  do
  {
    //0x00��һ���������ָ�� ������ָ����Ϊ������FLASHоƬ��Ҫ��ʱ��
    //���ҽ�FLASH��״̬�Ĵ�����ֵ���ص�Ƭ��	  ********�����Ǻܶ�
	FLASH_Status =  SpiSendData(0x00);
  }
  while ((FLASH_Status & 0x01) == 0x01);//��û��д����һֱ�ȴ�������CS=0

  W25_CS = 1;
}
///************************************************************************
//�������ƣ�Spi_PageWrite
//�������ܣ�Flash  ҳд
//��ڲ�����
//		tBuf---------->��������ָ��
//		tWriteAddr---->������Flash�ĵ�ַ		
//		tNum---------->�������ݸ���					
//*************************************************************************/
//unsigned int tgmwrit=0;
//unsigned int twrite1,twrite2;
//void Spi_PageWrite(unsigned char* tBuf,unsigned long tWriteAddr,unsigned int tNum)
//{
//	while(W25Q16_ReadStatus()&0x01);    //�ж��Ƿ�æ
//	SpiWriteEn();
//	W25_CS = 0;
////	f_spistatus=6;
//	SpiSendData(W25X_CMD_PageProgram);//ҳдָ��
//	SpiSendData((tWriteAddr & 0xff0000) >> 16);
//	SpiSendData((tWriteAddr & 0xff00) >> 8);
//	SpiSendData(tWriteAddr & 0xff);
//	while(tNum--)
//	{
//		SpiSendData(*tBuf);
//		tBuf++;
//	}
//	
//	W25_CS = 1;
//	SPI_FLASH_WaitForWriteEnd();
////	tSpiWriteAddr = tWriteAddr + tNum;//�´�д��ַ��2013-11-06  gm
//}
////�޼���дSPI FLASH
////����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
////�����Զ���ҳ����
////��ָ����ַ��ʼд��ָ�����ȵ�����
////pbuf:���ݴ洢��
////WriteAddr:��ʼд��ĵ�ַ(24bit)
////Len:Ҫд����ֽ���(���65535)
//void SPI_Flash_Write_NoCheck(unsigned char* pbuf,unsigned long WriteAddr,unsigned int Len)
//{
//    unsigned int PageLen;                  // ҳ��д���ֽڳ���
//    PageLen=256-WriteAddr%256;    // ��ҳʣ����ֽ��� ����ҳʣ��ռ䣩
//    if(Len<=PageLen) PageLen=Len; // ������256 ���ֽ�
//    while(1)
//    {
//        Spi_PageWrite(pbuf,WriteAddr,PageLen);
//        if(PageLen==Len)break;   // д�������
//        else
//        {
//            pbuf+=PageLen;
//            WriteAddr+=PageLen;
//            Len-=PageLen;              //  ��ȥ�Ѿ�д���˵��ֽ���
//            if(Len>256)PageLen=256;   // һ�ο���д��256 ���ֽ�
//            else PageLen=Len;          // ����256 ���ֽ���
//        }
//    }
//}
/************************************************************************
�������ƣ�Spi_PageRead
�������ܣ�Flash  ҳ��ȡ
��ڲ�����
		tBuf---------->�������ݻ�����ָ��
		tWriteAddr---->��ȡ��Flash��ַ	
		tNum---------->��ȡ���ݸ���					
*************************************************************************/
unsigned int tgmread=0;
unsigned int t_read,t_read1;
void Spi_PageRead(unsigned char* pBuf, unsigned long tReadAddr, unsigned int tNum)
{
	while(W25Q16_ReadStatus()&0x01); 
	W25_CS = 0;
//	f_spistatus=7;
	SpiSendData(W25X_CMD_ReadData);//���ݶ�ȡָ��
		
	SpiSendData((tReadAddr & 0xff0000) >> 16);
	SpiSendData((tReadAddr & 0xff00) >> 8);
	SpiSendData(tReadAddr & 0xff);
	
	while (tNum--) 
	{

		*pBuf = SpiSendData(0xff);
		pBuf++;

	}
	W25_CS = 1;
}
//
//void SDOpen(void)
//{
//	SD_CS=0;
//	SPCTL &=0xbf;  //��SD ,��W25Q64
//}
//
//void W25Open(void)
//{
//	SD_CS=1;
//	SPCTL|=SPEN;  //��SD ,��W25Q64
//}
