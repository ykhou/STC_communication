#include "fontcheck.h" 
#include "flash.h"  
#include "string.h"
#include "delay.h"
#include "chuankou.h"

//�ֿ�ṹΪ: info��Ϣ+UNIGBK.BIN+4���ֿ�
//�ֿ�����ռ�ֽ�����
//info��Ϣ     : 73
//U2G_G2U.BIN  : 174336
//ASC12        : 1542
//ASC16        : 2054
//ASC24        : 6150
//ASC32        : 8198
//GBK12        : 574560   һ����18���ֽ�
//GBK16        : 766080    32
//GBK24        : 1723680   72
//GBK32        : 3064320    128
//PC1           : 6050	  ����ͼƬȫ����6050
//���ֽ�: 6320993 �ֽ� ռ��������С,Լռ1544��W25QXX����,97���飨*64=6M������ʣ��2552��������*4k=9.9M���ࣩ
//������9M�ռ���ΪFATFS�ļ�ϵͳ,ʣ��0.9MΪ�û�����

#define FONTSECSIZE	 	1544
//�ֿ�����ʼ��ַ 
unsigned long FONT_ADDR=0x00;

_font_info ftinfo;


//�ֿ����ڴ����е�·��0:Ϊ���ص�TF���̷�
const unsigned char U2G_G2U_PATH[]="0:/SYSTEM/FONT/U2G_G2U.BIN";		//U2G_G2U.BIN�Ĵ��λ��
const unsigned char ASC12_PATH[]="0:/SYSTEM/FONT/ASC12.FON";		//ASC12���λ��
const unsigned char ASC16_PATH[]="0:/SYSTEM/FONT/ASC16.FON";		//ASC16���λ��
const unsigned char ASC24_PATH[]="0:/SYSTEM/FONT/ASC24.FON";		//ASC24���λ��
const unsigned char ASC32_PATH[]="0:/SYSTEM/FONT/ASC32.FON";		//ASC32���λ��

const unsigned char GBK12_PATH[]="0:/SYSTEM/FONT/GBK12.FON";		//GBK12�Ĵ��λ��
const unsigned char GBK16_PATH[]="0:/SYSTEM/FONT/GBK16.FON";		//GBK16�Ĵ��λ��
const unsigned char GBK24_PATH[]="0:/SYSTEM/FONT/GBK24.FON";		//GBK24�Ĵ��λ��
const unsigned char GBK32_PATH[]="0:/SYSTEM/FONT/GBK32.FON";		//GBK32�Ĵ��λ��

/***
****����FLASH�и����ֿ����ʼλ��
****/
void ftinfoInt(void)
{
	ftinfo.fontok = 0x00;   //
	ftinfo.u2g_addr = 0x0100; 			//unigbk�ĵ�ַ  ֱ��0x2AA00
	ftinfo.u2g_size	= 0x2A900;			//unigbk�Ĵ�С	 
		
	ftinfo.asc12_addr = 0x2B000;			//asc12��ַ	 ֱ��2B007
	ftinfo.asc12_size = 0x0606;			//asc12�Ĵ�С	 		
	ftinfo.asc16_addr = 0x2C000;			//asc16��ַ	  ֱ��2B80E
	ftinfo.asc16_size = 0x0806;			//asc16�Ĵ�С		
	ftinfo.asc24_addr = 0x2D000;			//asc24��ַ	  ֱ��2D015
	ftinfo.asc24_size = 0x1806;			//asc24�Ĵ�С		
	ftinfo.asc32_addr = 0x2EA00;			//asc32��ַ	 ֱ��2F01C
	ftinfo.asc32_size = 0x2006;			//asc32�Ĵ�С		
		
	ftinfo.gbk12_addr = 0x31000;			//gbk12��ַ	  ֱ��BB47D
	ftinfo.gbk12_size = 0x8C460;			//gbk12�Ĵ�С	 
	ftinfo.gbk16_addr = 0xBE000;			//gbk16��ַ  ֱ��1764FE
	ftinfo.gbk16_size = 0xBB080;			//gbk16�Ĵ�С		 
	ftinfo.gbk24_addr = 0x180000;			//gbk24��ַ  ֱ��31B21F
	ftinfo.gbk24_size = 0x1A4D20;			//gbk24�Ĵ�С 
	ftinfo.gbk32_addr = 0x326000;			//gbk24��ַ  ֱ��607420
	ftinfo.gbk32_size = 0x2EC200;

	ftinfo.pic1_addr = 0x615000;			//pic1��ַ		 6167a2
	ftinfo.pic1_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic2_addr = 0x617000;			//pic1��ַ	
	ftinfo.pic2_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic3_addr = 0x619000;			//pic1��ַ	
	ftinfo.pic3_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic4_addr = 0x61B000;			//pic1��ַ	
	ftinfo.pic4_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic5_addr = 0x61D000;			//pic1��ַ	
	ftinfo.pic5_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic6_addr = 0x61F000;			//pic1��ַ	
	ftinfo.pic6_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic7_addr = 0x621000;			//pic1��ַ	
	ftinfo.pic7_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic8_addr = 0x623000;			//pic1��ַ		 0x6247A2
	ftinfo.pic8_size = 0x17A2;			//pic2�Ĵ�С
	ftinfo.pic9_addr = 0x625000;			//pic1��ַ		 0x6247A2
  ftinfo.pic9_size = 0x17A2;			//pic2�Ĵ�С
}




























