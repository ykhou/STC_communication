#include "STC15F2K60S2.h"
#include <intrins.h>
#include  "delay.h"				  //延时函数
#include  "tft.h"
#include  "flash.h"
#include  "stdio.h"
#include  "chuankou.h"
/**单片机作为主机，W25Q64作为从机**/
sbit W25_CS = P5^0;
sbit NR_MOSI = P1^3;
sbit NR_MISO = P1^4;
sbit NR_SCK = P1^5;

sbit SD_CS   = P1^2;

unsigned int DEVICE_ID = 0xEF17;  //W25Q64的制造商标志
bit f_spistatus = 0;




/************************************************
SPI中断服务程序
//***********************************************/
//void spi_isr() interrupt 9
//{
//	SPSTAT = SPIF|WCOL;  //清除SPI状态位
//	f_spistatus = FALSE;
//}

/****************************************
SPI初始化		 初始化时未使能SPI
*****************************************/
void InitSpi(void)
{
    P1M1 |=(1<<4);
	P1M0 &=~(1<<4);

	P1M1 &=~(1<<3)|(1<<5);
	P1M0 |=(1<<3)|(1<<5);

	P5M1 &=~(1<<0);
	P5M0 |=(1<<0);

	ACC = P_SW1;				 //切换到第一组spi
	ACC &= ~(SPI_S0|SPI_S1);	 //SPI_S0=0;SPI_S1=0
	P_SW1 |= ACC;				 //(P1.2/SS,P1.3/MOSI,P1.4/MISO,P1.5/SCLK)

	SPSTAT = SPIF|WCOL;          //清除SPI状态
	W25_CS = 1;
	SPDAT = 0;		//清空SPI寄存器
	SPCTL = SPEN|SSIG|MSTR|SPDL;      //设置SPI为主模式,
									//注意：时钟选为CPU_CLK/16,选择CPU_CLK/4传输出错，建议时钟频率一般不要超过3MHz

//	IE2|=ESPI;
//	EA =1;
}
// 功能：读取W25Q16芯片的状态。
// 返回值：状态寄存器数据字节
// 注：W25X16内部状态寄存器第0位=0表示空闲，0位=1表示忙。
unsigned char W25Q16_ReadStatus()
{
    unsigned char status=0;
    W25_CS=0;
    SpiSendData(W25X_CMD_ReadStatusReg1);   // 0x05读取状态的命令字
    status=SpiSendData(0xff);        // 读取状态字节
    W25_CS=1;                         // 关闭片选
    return status;
} 
/**********************************************************************
函数名称：ReadDeviceID
函数功能：读取Device ID
入口参数：无
出口参数：
            DeviceID-------->设备ID
***********************************************************************/
unsigned int ReadDeviceID(void)
{
    unsigned int DeviceID=0;
    while(W25Q16_ReadStatus()&0x01);    //判断是否忙
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
函数名称：SpiSendData
函数功能：SPI发送数据
入口参数：
          tData----------------->待发送数据
出口参数：无
************************************************************************/

unsigned char SpiSendData(unsigned char tData)
{  
    SPDAT = tData;			//发送缓冲区为空时发送数据  
    while(!(SPSTAT & 0x80));//等待发送完毕
	SPSTAT = SPIF|WCOL;//设置状态寄存器初值
	return SPDAT;
}

void Spi_Flash_init(void)
{
//	DEVICE_ID = ReadDeviceID();//读取Flash ID
	Flash_ChipErase();//Flash 擦除
}

/**********************************************************************
函数名称：Flash_32K_Erase
函数功能：32K 区域擦除
入口参数：
出口参数：
***********************************************************************/
void Flash_64K_Erase(unsigned long EraseAddr)
{

//	EraseAddr*=W25Q64_BLOCK_SIZE;
	SpiWriteEn();
	
	W25_CS = 0;
	
	SpiSendData(0xd8);  //64K区域擦除
	SpiSendData((EraseAddr & 0xFF0000) >> 16);
	SpiSendData((EraseAddr & 0xFF00) >> 8);
	SpiSendData(EraseAddr & 0xFF);
	W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}  
/**********************************************************************
函数名称：Flash_4K_Erase
函数功能：4K 区域擦除  扇区擦除
入口参数：
出口参数：
***********************************************************************/
void Flash_4K_Erase(unsigned long EraseAddr)
{

//    EraseAddr*=W25Q64_SECTOR_SIZE;
    SpiWriteEn();
    
    W25_CS = 0;
    
    SpiSendData(0x20);  //4K区域擦除
    SpiSendData((EraseAddr & 0xFF0000) >> 16);
    SpiSendData((EraseAddr & 0xFF00) >> 8);
    SpiSendData(EraseAddr & 0xFF);
    W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}

/*********************************************************************
函数名称：Flash_ChipErase
函数功能：整片擦除                 					  
*********************************************************************/                      
void Flash_ChipErase(void)
{
	SpiWriteEn(); //写使能
	W25_CS = 0;
//	f_spistatus=2;
	SpiSendData(W25X_CMD_ChipErase);   //发送整片擦除指令
	W25_CS = 1;
	SPI_FLASH_WaitForWriteEnd();
}
/***********************************************************************
函数功能：SPI延时
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
函数名称：SpiWriteEn
函数功能：SPI写使能
入口参数：无
出口参数：无
************************************************************************/
void SpiWriteEn(void)
{
    W25_CS = 0;
//  SpiDelay();
//	f_spistatus=3;
    SpiSendData(W25X_CMD_WriteEnable);	 //发送写使能指令
    W25_CS = 1;
}   
/**************************************************************************
函数名称：SPI_FLASH_WaitForWriteEnd
函数功能：读Flash芯片状态寄存器的值并且循环读取标志位直到写周期结束
入口参数：无
出口参数：无
 *************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;
  W25_CS = 0;

  // 发送读状态寄存器命令，发送完之后，状态寄存器的值会被传送到STM8 
//  f_spistatus=4;
  SpiSendData(W25X_CMD_ReadStatusReg1); //读取Flash 状态寄存器1

  // 循环查询标志位 等待写周期结束 
  do
  {
    //0x00是一个无意义的指令 发这条指令是为了生成FLASH芯片需要的时钟
    //并且将FLASH的状态寄存器的值读回单片机	  ********并不是很懂
	FLASH_Status =  SpiSendData(0x00);
  }
  while ((FLASH_Status & 0x01) == 0x01);//若没有写完则一直等待，保持CS=0

  W25_CS = 1;
}
///************************************************************************
//函数名称：Spi_PageWrite
//函数功能：Flash  页写
//入口参数：
//		tBuf---------->待发数据指针
//		tWriteAddr---->发送至Flash的地址		
//		tNum---------->发送数据个数					
//*************************************************************************/
//unsigned int tgmwrit=0;
//unsigned int twrite1,twrite2;
//void Spi_PageWrite(unsigned char* tBuf,unsigned long tWriteAddr,unsigned int tNum)
//{
//	while(W25Q16_ReadStatus()&0x01);    //判断是否忙
//	SpiWriteEn();
//	W25_CS = 0;
////	f_spistatus=6;
//	SpiSendData(W25X_CMD_PageProgram);//页写指令
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
////	tSpiWriteAddr = tWriteAddr + tNum;//下次写地址。2013-11-06  gm
//}
////无检验写SPI FLASH
////必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
////具有自动换页功能
////在指定地址开始写入指定长度的数据
////pbuf:数据存储区
////WriteAddr:开始写入的地址(24bit)
////Len:要写入的字节数(最大65535)
//void SPI_Flash_Write_NoCheck(unsigned char* pbuf,unsigned long WriteAddr,unsigned int Len)
//{
//    unsigned int PageLen;                  // 页内写入字节长度
//    PageLen=256-WriteAddr%256;    // 单页剩余的字节数 （单页剩余空间）
//    if(Len<=PageLen) PageLen=Len; // 不大于256 个字节
//    while(1)
//    {
//        Spi_PageWrite(pbuf,WriteAddr,PageLen);
//        if(PageLen==Len)break;   // 写入结束了
//        else
//        {
//            pbuf+=PageLen;
//            WriteAddr+=PageLen;
//            Len-=PageLen;              //  减去已经写入了的字节数
//            if(Len>256)PageLen=256;   // 一次可以写入256 个字节
//            else PageLen=Len;          // 不够256 个字节了
//        }
//    }
//}
/************************************************************************
函数名称：Spi_PageRead
函数功能：Flash  页读取
入口参数：
		tBuf---------->接收数据缓冲区指针
		tWriteAddr---->读取的Flash地址	
		tNum---------->读取数据个数					
*************************************************************************/
unsigned int tgmread=0;
unsigned int t_read,t_read1;
void Spi_PageRead(unsigned char* pBuf, unsigned long tReadAddr, unsigned int tNum)
{
	while(W25Q16_ReadStatus()&0x01); 
	W25_CS = 0;
//	f_spistatus=7;
	SpiSendData(W25X_CMD_ReadData);//数据读取指令
		
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
//	SPCTL &=0xbf;  //开SD ,关W25Q64
//}
//
//void W25Open(void)
//{
//	SD_CS=1;
//	SPCTL|=SPEN;  //关SD ,开W25Q64
//}
