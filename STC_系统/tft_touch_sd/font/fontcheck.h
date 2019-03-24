#ifndef __FONTCHECK_H__
#define __FONTCHECK_H__	 

//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
//typedef __packed struct   //packedָ������Ҫ����
typedef struct 
{
	unsigned char fontok;				//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
//  u8 info_state;     //�ֿ���ڱ�־,info_state&(1<<7)-u2g info_state&(1<<7)   0xAA����;����������
//	u8 font_state;     //ascii�ֿ��Ƿ���� asc_state&(1<<0)-asc12;asc_state&(1<<1)-asc16; asc_state&(1<<2)-asc24; asc_state&(1<<3)-asc32
//     //gbk�ֿ��Ƿ���� asc_state&(1<<4)-gbk12;asc_state&(1<<5)-gbk16; asc_state&(1<<6-gbk24; asc_state&(1<<7)-gbk32
	
	unsigned long u2g_addr; 			//unigbk�ĵ�ַ
	unsigned long u2g_size;			//unigbk�Ĵ�С	 
	
	unsigned long asc12_addr;			//asc12��ַ	
	unsigned long asc12_size;			//asc12�Ĵ�С	 		
	unsigned long asc16_addr;			//asc16��ַ	
	unsigned long asc16_size;			//asc16�Ĵ�С		
	unsigned long asc24_addr;			//asc24��ַ	
	unsigned long asc24_size;			//asc24�Ĵ�С		
	unsigned long asc32_addr;			//asc32��ַ	
	unsigned long asc32_size;			//asc32�Ĵ�С		
	
	unsigned long gbk12_addr;			//gbk12��ַ	
	unsigned long gbk12_size;			//gbk12�Ĵ�С	 
	unsigned long gbk16_addr;			//gbk16��ַ
	unsigned long gbk16_size;			//gbk16�Ĵ�С		 
	unsigned long gbk24_addr;			//gbk24��ַ
	unsigned long gbk24_size;			//gbk24�Ĵ�С 
	unsigned long gbk32_addr;			//gbk24��ַ
	unsigned long gbk32_size;			//gbk24�Ĵ�С
	
	unsigned long pic1_addr;			//pic1��ַ	
	unsigned long pic1_size;			//pic2�Ĵ�С
	unsigned long pic2_addr;			//pic1��ַ	
	unsigned long pic2_size;			//pic2�Ĵ�С
	unsigned long pic3_addr;			//pic1��ַ	
	unsigned long pic3_size;			//pic2�Ĵ�С
	unsigned long pic4_addr;			//pic1��ַ	
	unsigned long pic4_size;			//pic2�Ĵ�С
	unsigned long pic5_addr;			//pic1��ַ	
	unsigned long pic5_size;			//pic2�Ĵ�С
	unsigned long pic6_addr;			//pic1��ַ	
	unsigned long pic6_size;			//pic2�Ĵ�С	 
	unsigned long pic7_addr;			//pic1��ַ	
	unsigned long pic7_size;			//pic2�Ĵ�С
	unsigned long pic8_addr;			//pic1��ַ	
	unsigned long pic8_size;			//pic2�Ĵ�С
	unsigned long pic9_addr;			//pic1��ַ	
	unsigned long pic9_size;			//pic2�Ĵ�С	
}_font_info; 

extern _font_info ftinfo;	//�ֿ���Ϣ�ṹ��

void ftinfoInt(void);
void check_SDcard_font(void);

#endif
