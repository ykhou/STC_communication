#ifndef __FONTCHECK_H__
#define __FONTCHECK_H__	 

//字库信息结构体定义
//用来保存字库基本信息，地址，大小等
//typedef __packed struct   //packed指定不需要对齐
typedef struct 
{
	unsigned char fontok;				//字库存在标志，0XAA，字库正常；其他，字库不存在
//  u8 info_state;     //字库存在标志,info_state&(1<<7)-u2g info_state&(1<<7)   0xAA存在;其他不存在
//	u8 font_state;     //ascii字库是否存在 asc_state&(1<<0)-asc12;asc_state&(1<<1)-asc16; asc_state&(1<<2)-asc24; asc_state&(1<<3)-asc32
//     //gbk字库是否存在 asc_state&(1<<4)-gbk12;asc_state&(1<<5)-gbk16; asc_state&(1<<6-gbk24; asc_state&(1<<7)-gbk32
	
	unsigned long u2g_addr; 			//unigbk的地址
	unsigned long u2g_size;			//unigbk的大小	 
	
	unsigned long asc12_addr;			//asc12地址	
	unsigned long asc12_size;			//asc12的大小	 		
	unsigned long asc16_addr;			//asc16地址	
	unsigned long asc16_size;			//asc16的大小		
	unsigned long asc24_addr;			//asc24地址	
	unsigned long asc24_size;			//asc24的大小		
	unsigned long asc32_addr;			//asc32地址	
	unsigned long asc32_size;			//asc32的大小		
	
	unsigned long gbk12_addr;			//gbk12地址	
	unsigned long gbk12_size;			//gbk12的大小	 
	unsigned long gbk16_addr;			//gbk16地址
	unsigned long gbk16_size;			//gbk16的大小		 
	unsigned long gbk24_addr;			//gbk24地址
	unsigned long gbk24_size;			//gbk24的大小 
	unsigned long gbk32_addr;			//gbk24地址
	unsigned long gbk32_size;			//gbk24的大小
	
	unsigned long pic1_addr;			//pic1地址	
	unsigned long pic1_size;			//pic2的大小
	unsigned long pic2_addr;			//pic1地址	
	unsigned long pic2_size;			//pic2的大小
	unsigned long pic3_addr;			//pic1地址	
	unsigned long pic3_size;			//pic2的大小
	unsigned long pic4_addr;			//pic1地址	
	unsigned long pic4_size;			//pic2的大小
	unsigned long pic5_addr;			//pic1地址	
	unsigned long pic5_size;			//pic2的大小
	unsigned long pic6_addr;			//pic1地址	
	unsigned long pic6_size;			//pic2的大小	 
	unsigned long pic7_addr;			//pic1地址	
	unsigned long pic7_size;			//pic2的大小
	unsigned long pic8_addr;			//pic1地址	
	unsigned long pic8_size;			//pic2的大小
	unsigned long pic9_addr;			//pic1地址	
	unsigned long pic9_size;			//pic2的大小	
}_font_info; 

extern _font_info ftinfo;	//字库信息结构体

void ftinfoInt(void);
void check_SDcard_font(void);

#endif
