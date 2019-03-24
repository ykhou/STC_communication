#ifndef __EEPROM_H
#define __EEPROM_H	

//#define NULL 0
#define FALSE 0
#define TRUE 1

#define SPI_S0 0X04
#define SPI_S1 0X08
#define SPIF 0X80  //SPSTAT.7
#define WCOL 0X40  //SPSTAT.6
#define SSIG 0X80  //SPCTL.7
#define SPEN 0X40  //SPCTL.6
#define DORD 0X20  //SPCTL.5
#define MSTR 0X10  //SPCTL.4
#define CPOL 0X08  //SPCTL.3
#define CPHA 0X04  //SPCTL.2
#define SPDHH 0X00	//CPU_CLK/4
#define SPDH 0X01  //CPU_CLK/8
#define SPDL 0X02  //CPU_CLK/16
#define SPDLL 0X03  //CPU_CLK/32
#define ESPI 0X02  //IE2.1

#define tSpiWriteAddr 0x00000000

#define W25Q64_DeviceID 0xF7C2
 
#define W25X_CMD_WriteEnable 0x06   /*Write enable instruction */
#define W25X_CMD_WriteDisable 0x04 /*! Write to Memory Disable */
#define W25X_CMD_WriteStatusReg 0x01 /* Write Status Register instruction */
 
#define W25X_CMD_PageProgram 0x02 /* Write enable instruction */
#define W25X_CMD_QuadPageProgram 0x32 /* Write enable instruction */
 
#define W25X_CMD_BlockErase64 0xD8 /* Block 64k Erase instruction */
#define W25X_CMD_BlockErase32 0x52 /* Block 32k Erase instruction */
#define W25X_CMD_ChipErase 0xC7 /* Bulk Erase instruction */
#define W25X_CMD_SectorErase 0x20 /* Sector 4k Erase instruction */
#define W25X_CMD_EraseSuspend 0x75 /* Sector 4k Erase instruction */
#define W25X_CMD_EraseResume 0x7a /* Sector 4k Erase instruction */
 
#define W25X_CMD_ReadStatusReg1 0x05 /* Read Status Register instruction */
#define W25X_CMD_ReadStatusReg2 0x35 /* Read Status Register instruction */
 
#define W25X_CMD_High_Perform_Mode 0xa3
#define W25X_CMD_Conti_Read_Mode_Ret 0xff
 
#define W25X_WakeUp 0xAB
#define W25X_JedecDeviveID 0x9F /*Read identification */
#define W25X_ManufactDeviveID 0x90 /* Read identification */
#define W25X_ReadUniqueID 0x4B
 
#define W25X_Power_Down 0xB9 /*Sector 4k Erase instruction */
 
#define W25X_CMD_ReadData 0x03 /* Read from Memory instruction */
#define W25X_CMD_FastRead 0x0b /* Read from Memory instruction */
#define W25X_CMD_FastReadDualOut 0x3b /*Read from Memory instruction */
#define W25X_CMD_FastReadDualIO 0xBB /* Read from Memory instruction */
#define W25X_CMD_FastReadQuadOut 0x6b /* Read from Memory instruction */
#define W25X_CMD_FastReadQuadIO 0xeb /* Read from Memory instruction */
#define W25X_CMD_OctalWordRead 0xe3 /* Read from Memory instruction */
 
#define W25X_DUMMY_BYTE 0xff //0xA5
#define W25X_SPI_PAGESIZE 0x100

#define u8 unsigned char
#define u16 unsigned int	
#define u32 unsigned long


void InitSpi(void);
unsigned char SpiSendData(unsigned char tData);
unsigned int ReadDeviceID(void);
//void ReadDeviceID(void);
void Spi_Flash_init(void);
void Flash_ChipErase(void);
void SpiDelay(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SpiWriteEn(void);
void Spi_PageWrite(unsigned char* tBuf,unsigned long tWriteAddr,unsigned int tNum);
void Spi_PageRead(unsigned char* pBuf, unsigned long tReadAddr, unsigned int tNum);
unsigned char W25Q16_ReadStatus();
void SPI_Flash_Write_NoCheck(unsigned char* pbuf,unsigned long WriteAddr,unsigned int Len);
void SDRred_W25Write(void);
void Flash_4K_Erase(unsigned long EraseAddr);

void SDOpen(void);
void W25Open(void);
#endif