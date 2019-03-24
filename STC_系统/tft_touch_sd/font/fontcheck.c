#include "fontcheck.h" 
#include "flash.h"  
#include "string.h"
#include "delay.h"
#include "chuankou.h"

//字库结构为: info信息+UNIGBK.BIN+4个字库
//字库区域占字节数：
//info信息     : 73
//U2G_G2U.BIN  : 174336
//ASC12        : 1542
//ASC16        : 2054
//ASC24        : 6150
//ASC32        : 8198
//GBK12        : 574560   一个字18个字节
//GBK16        : 766080    32
//GBK24        : 1723680   72
//GBK32        : 3064320    128
//PC1           : 6050	  九张图片全部是6050
//总字节: 6320993 字节 占扇区数大小,约占1544个W25QXX扇区,97个块（*64=6M），还剩余2552个扇区（*4k=9.9M空余）
//接下来9M空间作为FATFS文件系统,剩余0.9M为用户自用

#define FONTSECSIZE	 	1544
//字库存放起始地址 
unsigned long FONT_ADDR=0x00;

_font_info ftinfo;


//字库存放在磁盘中的路径0:为挂载的TF卡盘符
const unsigned char U2G_G2U_PATH[]="0:/SYSTEM/FONT/U2G_G2U.BIN";		//U2G_G2U.BIN的存放位置
const unsigned char ASC12_PATH[]="0:/SYSTEM/FONT/ASC12.FON";		//ASC12存放位置
const unsigned char ASC16_PATH[]="0:/SYSTEM/FONT/ASC16.FON";		//ASC16存放位置
const unsigned char ASC24_PATH[]="0:/SYSTEM/FONT/ASC24.FON";		//ASC24存放位置
const unsigned char ASC32_PATH[]="0:/SYSTEM/FONT/ASC32.FON";		//ASC32存放位置

const unsigned char GBK12_PATH[]="0:/SYSTEM/FONT/GBK12.FON";		//GBK12的存放位置
const unsigned char GBK16_PATH[]="0:/SYSTEM/FONT/GBK16.FON";		//GBK16的存放位置
const unsigned char GBK24_PATH[]="0:/SYSTEM/FONT/GBK24.FON";		//GBK24的存放位置
const unsigned char GBK32_PATH[]="0:/SYSTEM/FONT/GBK32.FON";		//GBK32的存放位置

/***
****定义FLASH中各个字库的起始位置
****/
void ftinfoInt(void)
{
	ftinfo.fontok = 0x00;   //
	ftinfo.u2g_addr = 0x0100; 			//unigbk的地址  直到0x2AA00
	ftinfo.u2g_size	= 0x2A900;			//unigbk的大小	 
		
	ftinfo.asc12_addr = 0x2B000;			//asc12地址	 直到2B007
	ftinfo.asc12_size = 0x0606;			//asc12的大小	 		
	ftinfo.asc16_addr = 0x2C000;			//asc16地址	  直到2B80E
	ftinfo.asc16_size = 0x0806;			//asc16的大小		
	ftinfo.asc24_addr = 0x2D000;			//asc24地址	  直到2D015
	ftinfo.asc24_size = 0x1806;			//asc24的大小		
	ftinfo.asc32_addr = 0x2EA00;			//asc32地址	 直到2F01C
	ftinfo.asc32_size = 0x2006;			//asc32的大小		
		
	ftinfo.gbk12_addr = 0x31000;			//gbk12地址	  直到BB47D
	ftinfo.gbk12_size = 0x8C460;			//gbk12的大小	 
	ftinfo.gbk16_addr = 0xBE000;			//gbk16地址  直到1764FE
	ftinfo.gbk16_size = 0xBB080;			//gbk16的大小		 
	ftinfo.gbk24_addr = 0x180000;			//gbk24地址  直到31B21F
	ftinfo.gbk24_size = 0x1A4D20;			//gbk24的大小 
	ftinfo.gbk32_addr = 0x326000;			//gbk24地址  直到607420
	ftinfo.gbk32_size = 0x2EC200;

	ftinfo.pic1_addr = 0x615000;			//pic1地址		 6167a2
	ftinfo.pic1_size = 0x17A2;			//pic2的大小
	ftinfo.pic2_addr = 0x617000;			//pic1地址	
	ftinfo.pic2_size = 0x17A2;			//pic2的大小
	ftinfo.pic3_addr = 0x619000;			//pic1地址	
	ftinfo.pic3_size = 0x17A2;			//pic2的大小
	ftinfo.pic4_addr = 0x61B000;			//pic1地址	
	ftinfo.pic4_size = 0x17A2;			//pic2的大小
	ftinfo.pic5_addr = 0x61D000;			//pic1地址	
	ftinfo.pic5_size = 0x17A2;			//pic2的大小
	ftinfo.pic6_addr = 0x61F000;			//pic1地址	
	ftinfo.pic6_size = 0x17A2;			//pic2的大小
	ftinfo.pic7_addr = 0x621000;			//pic1地址	
	ftinfo.pic7_size = 0x17A2;			//pic2的大小
	ftinfo.pic8_addr = 0x623000;			//pic1地址		 0x6247A2
	ftinfo.pic8_size = 0x17A2;			//pic2的大小
	ftinfo.pic9_addr = 0x625000;			//pic1地址		 0x6247A2
  ftinfo.pic9_size = 0x17A2;			//pic2的大小
}




























