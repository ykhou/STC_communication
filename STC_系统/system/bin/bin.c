#include "STC15F2K60S2.h"
#include "bin.h"
#include "tft.h"
#include "flash.h"

/******************************
��ʾƴͼ ͼƬ
������ָ��λ�� ��ʾָ����С��binͼƬ
ͨ���ɼ�����ÿ��ƴͼ���ڵĿ�ʼ�غ� 
������ʾ����
x1 y1��ʼ����  width height ��ʾ�Ŀ�Ⱥ͸߶�  

cjdΪ��ƴͼ�ļ����� �ɼ���ͼƬ���� �ǰ�˳�����е� �����0 ��ʾͼƬ�� 1���ʾͼƬ1
   ���������ֱ�Ӵ��뵽	doc_info.DOC_FstClusL[cjd]�� �͵õ�ͼƬ�Ŀ�ʼ�غ� 
   ����ͼƬ�Ķ�Ӧ�غ� ��SD����ɼ�ͼƬ����Ӧ����
*******************************/
void DISP_BIN_PT_W25(unsigned int x1,unsigned int y1,unsigned int width,unsigned int height,unsigned long tReadAddr1,unsigned long tReadAddr2)
{
	unsigned char tmp_Char_Model[2];
	unsigned long total=0;
	unsigned long i;
//	unsigned long Temp_ClusL1,new_Sec1;
	unsigned int x,y;
//	unsigned int i1;
	x=x1; y=y1;
//	Temp_ClusL1= doc_info.PIC_FstClusL[cjd];         //���״ظ�ֵ����ʱ���� 
//	while(Temp_ClusL1!=0x0fffffff)                   //һֱ�����һ��
//	{
//		new_Sec1=Cluster_to_Sector(Temp_ClusL1);	    //��غŵ�������  =(ĳ�غ�-��Ŀ¼�غ�)*ÿ��������+��Ŀ¼��ʼ������
//		for(j1=0;j1<fat_info.BPB_perCluster_Sector_number;j1++)     //��ÿ�ص���������  ÿ�������� ��SD���������
//		{
//		   SD_read_sector(new_Sec1,buffer);						    //��һ������������ 
//    	  for(i1=0;i1<256;i1++)                          //Ȼ��д��Һ������������ʾ256�����أ�ÿ������16λ��2���ֽ�?Ҳ����ÿ�����ֽ�����ʾһ�����ص� 
//	      {  
//		    total+=2;
//       	    if(total>doc_info.PIC_Filesize[cjd])return;	 //�ж��Ƿ����һ��  ����� ��������
		  
	for(i=0;i<(tReadAddr2-tReadAddr1)/2;i++)
	{	  	
		Spi_PageRead(tmp_Char_Model,(unsigned long)tReadAddr1+i,2);
			TFT_AddressSet(x,y,x,y);                     //����һ����һ�������ʾ ��ɨ���� ��ɨ����	   Ҫ����BIN ���ɷ�ʽ �����������
		   TFT_WR_DATA8(tmp_Char_Model[1],tmp_Char_Model[0]);
	
	        x++;								//�����Լ� ����һ�� �������� �����Լ� ֱ��ɨ��������Ļ
	        if(x==width+x1)                         //����Ƿ�д�����ı�Ե 240X320
	        {	 
	        y++;
	        x=x1;
	        if(y==height+y1)	y=y1; 	          
	        }
		i++;
	}
//		   	j1=0;
//			new_Sec1++;     						//������+1
//		}
//		 Temp_ClusL1=FAT_seek_nextCluster(Temp_ClusL1);//��FAT���в�����һ���غ�	 
//   }
}


void DISP_BIN_PT(unsigned int x1,unsigned int y1,unsigned int width,unsigned int height,unsigned char *tmp_Char_Model)
{
	unsigned long len=width*height*2;//��ɫ�����ֽ�
	unsigned long i;
	unsigned int x,y;
	x=x1; y=y1;
		  
	for(i=0;i<len/2;i++)
	{	  	
	
			TFT_AddressSet(x,y,x,y);                     //����һ����һ�������ʾ ��ɨ���� ��ɨ����	   Ҫ����BIN ���ɷ�ʽ �����������
			TFT_WR_DATA8(tmp_Char_Model[i*2+1],tmp_Char_Model[2*i]);
	//	TFT_WR_DATA8(0xf8,0);

	        x++;								//�����Լ� ����һ�� �������� �����Լ� ֱ��ɨ��������Ļ
	        if(x==width+x1)                         //����Ƿ�д�����ı�Ե 240X320
	        {	 
						y++;
						x=x1;
						if(y==height+y1)	y=y1; 	          
	        }
	}

}
