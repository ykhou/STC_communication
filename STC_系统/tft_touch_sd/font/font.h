#ifndef __FONT_H
#define __FONT_H		
extern unsigned char code ASCII_6x8[][6];
extern unsigned char code ASCII_816[96][16];
struct typFNT_GB16
{
	unsigned char Index[3];		 //�ַ�������'\0'��β,����ռ2�ֽڡ�����3���ֽ�
	unsigned char MSK[32];
};
extern struct typFNT_GB16 code GB16[1];	//�ṹ���������ⲿ����ʱҪ�����ִ��ϣ���������				  extern unsigned char code write_chinese[][24];		 
struct typFNT_GB12x16
{
	unsigned char Index[3];		 //�ַ�������'\0'��β,����ռ2�ֽڡ�����3���ֽ�
	unsigned char MSK[24];
};
extern struct typFNT_GB12x16 code GB12x16[1];	//�ṹ���������ⲿ����ʱҪ�����ִ��ϣ���������				  		 
#endif  
	 
	 



