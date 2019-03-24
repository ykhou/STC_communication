#include "string.h"
#include "STC15F2K60S2.h"
#include "txt.h"
#include "tft.h"
#include "font.h"
#include "delay.h"
#include "flash.h"
#include "fontcheck.h"

extern unsigned char Y_LEN;//ȫ�ֱ���������Ϣһֱ���´�ӡ

void TFT_ShowWord_W25(unsigned int x,unsigned int y,unsigned int size_x,unsigned int size_y,unsigned char *Disp_char,unsigned int PColor,unsigned int BColor)
{
    unsigned int i,j,xCount=0;
	unsigned int pxSize;
	TFT_AddressSet(x,y,x+size_x-1,y+size_y-1); //�������� 
	pxSize =(size_x/8+(size_x%8?1:0))*size_y;   //�����ַ����������

	for(j=0;j<pxSize;j++)	  //size_x*size_y/8Ϊ�ֽڸ���
	{
		for(i=0;i<8;i++)	    //��λ��ǰ����λ�ں�ȡ��ģʱƥ���OK
		{ 						
			if(*Disp_char&(0x80>>i))  //���λΪ����7λ
			{
				//��Ӧ��ȡ��ģʱ������
				TFT_WR_DATA(PColor);
			} 
			else
			{				
				TFT_WR_DATA(BColor);
			}
			++xCount;	
			if(xCount%size_x==0){xCount=0;break;} //�����ַ�����8�������ַ�
		}
	  	Disp_char++;
	 }		
}
/**
**size������ѡ1��2��3��4��������ֵ��Ч
**/
void TFT_ShowString_W25(unsigned int x,unsigned int y,unsigned int size,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor)
{         
 	unsigned char i,val=0; //һ��Ҫ��ʼ������Ȼxlen�ڵڶ��ε���ʱ��������

	unsigned char temp[2]={0};
	unsigned int xlen;
	unsigned int ylen;	
  unsigned int max_ylen;
	unsigned int max_xlen;
	unsigned long pxSize=0,px;	  //�������
	unsigned long position=0;	
	unsigned char p[129]={0};	//32*32����4*32=128���ֽ�	 
	unsigned char p0[65]={0};

	if(size==1) px=12;
	else if(size==2) px=16;
	else if(size==3) px=24;
	//else if(size==4) px=28
	else if(size==4) px=32;
	else return;  										  	

  max_xlen=240*2/px;
	max_ylen=320/px;      	//�����ʾ240/8=30�� 320/16=20�� 
																								  
	for(i=0;i<strlen(ptr);i++)
	{
	 	temp[0]=ptr[i];
		temp[1]=ptr[i+1];
		
		xlen =(x+i+val)%max_xlen;	   	 		
		ylen =((x+i+val)/max_xlen+y)%max_ylen;  


		if(temp[0]<128)		//ΪASCII��
		{
			pxSize =(px/16+(px%16?1:0))*px;  //�����ַ����������		   ?????
			position=(unsigned long)temp[0]*(unsigned long)pxSize;
			if(px==12)	 { Spi_PageRead(p, ftinfo.asc12_addr+position, pxSize); } 
			else if(px==16){ Spi_PageRead(p, ftinfo.asc16_addr+position, pxSize); }
			else if(px==24){ Spi_PageRead(p, ftinfo.asc24_addr+position, pxSize); }
			else if(px==32){ Spi_PageRead(p, ftinfo.asc32_addr+position, pxSize); } 	
			TFT_ShowWord_W25(xlen*px/2,ylen*px,px/2,px,p,PenColor,BackColor); 	
		}
		else 			   //Ϊ����
	   	{	
			if(xlen==max_xlen-1)   //���һ�в��ܷ���һ������
			{
				TFT_ShowWord_W25(xlen*px/2,ylen*px,px/2,px,p0,PenColor,BackColor);  //��ʾ�� ���հ��
				val++;	
				i--;//ָ�뻹��ָ��ԭ����,����Ҫ��һ			
			}
			else
			{	//��һ������GBK���뷶ΧΪ0x81~0xFE
		    	//�ڶ�������GBK���뷶ΧΪ0x40~0x7E 0x80~0xFE
				pxSize =(px/8+(px%8?1:0))*px;   //�����ַ����������			??????????????		
				if(temp[1]>0x7F) 	  
					position=((unsigned long)(temp[0]-0x81)*190+(unsigned long)temp[1]-0x41)*pxSize;
				else if(temp[1]<0x7F)
					position=(((unsigned long)temp[0]-0x81)*190+(unsigned long)temp[1]-0x40)*pxSize;
			    else return;																							   
				
				if(px==12)	 { Spi_PageRead(p, ftinfo.gbk12_addr+position, pxSize); }
				else if(px==16){ Spi_PageRead(p, ftinfo.gbk16_addr+position, pxSize); }
				else if(px==24){ Spi_PageRead(p, ftinfo.gbk24_addr+position, pxSize); }
				else if(px==32){ Spi_PageRead(p, ftinfo.gbk32_addr+position, pxSize); } 				
				
				TFT_ShowWord_W25(xlen*px/2,ylen*px,px,px,p,PenColor,BackColor);	
				i++;		
			}
		 }
	}
	Y_LEN = ylen+1; //��¼���õ�������+��һ��
	Y_LEN %=max_ylen;
}


///*********************************************************************
//����TXT�ĺ��ĳ���
//����:�ļ��Ŀ�ʼ�غţ���С���ļ�������Щ����Ҫ����
//����:
//����ֵ:
//����ֵ:TXT�ļ�����
//*********************************************************************/
//void DISP_TXT(unsigned char cjd)
//{
//	unsigned long Temp_ClusL,new_Sec;
//	
//	unsigned int i,j=0;
//	unsigned long total=0;
//	unsigned char tmp_char[2],CHINA_FLAG=0;
//	unsigned char x=0,y=0;
//	unsigned char max_x=30,max_y=20;//�����ʾ240/8=30�� 320/16=20�� 	
//	TFT_ShowString_SD(0,19,"                        ��һҳ",WHITE,RED);
//	y=1;
//	Temp_ClusL= doc_info.DOC_FstClusL[cjd];//���״ظ�ֵ 
//	while(Temp_ClusL!=0x0fffffff)//һֱ�����һ��
//	{
//		new_Sec=Cluster_to_Sector(Temp_ClusL);	    //��غŵ�������  =(ĳ�غ�-��Ŀ¼�غ�)*ÿ��������+��Ŀ¼��ʼ������
//		for(i=0;i<fat_info.BPB_perCluster_Sector_number;i++)//��ÿ�ص���������
//		{
//                   //* ��һ��������
//		    SD_read_sector(new_Sec,buffer);
//			while(j<512)
//			{       
//			  	//* �����ʾ���ļ����˳� 
//				total++;
//        		if(total>doc_info.DOC_Filesize[cjd])
//				{
//					TFT_ShowString_SD(0,19,"                          ����",WHITE,RED);
//                   	while(1)
//					{
//	                    if(Convert_Pos())	//�õ�����ֵ
//						{	
//							if(tp_pixlcd.x>208&&tp_pixlcd.y>298)	  //�±�ǩ��"����"
//								return;
//						}
//					}
//				}
//				 //*�س��жϺ���  ������ֻ������ ���������ж�3���س�    
//             //�����ASCII��
//       			if(buffer[j] < 0x80)
//                {                            
//  		               if(buffer[j]==0x0d&&j==511) break;			//�жϻس��������ֽ��Ƿ������� �����ĽӺ�֮��   
//					   if(buffer[j]==0x0a&&j==0){j++;y++;x=0;}	//�������ܻ��ж��ֽ�����		
//		
//					  if((buffer[j]==0x0d)&&(buffer[j+1]==0x0a))    //�ж��Ƿ��лس� 
//					   {
//					     j=j+2;								   //�ֽ�+2 �����˻س����� 
//					     y++;								   // ����
//					      x=0;								   //�ӵ�һ����ʾλ�ÿ�ʼ��ʾ
//				    	 }
//					  else									   //��ʾ�ֽ�
//					    {                                                     
//                 	
//					TFT_ShowWord(x*8,y*16,8,16,ASCII_816[buffer[j]-0x20],BLACK,WHITE); 
//                    j++;            
//                    x++;  	               	
//               // �������һ��                                     
//                    if(x >= max_x)
//                    {
//                    	x = 0;
//                        y ++;
//                    }
//					}
//				}
//               //����Ǻ���/
//       			else //	  CHINA_FLAG �ô˺�������־ ���ж��Ƿ�Ϊ������	
//       			{	//ֻ��Ϊ�����ֲſ�������ʾ	�������Ա������		 
//					if(!CHINA_FLAG)				   
//               		{       
//                		 //���ֵ�����ֻ��¼���ݣ�����ʾ
//                       	tmp_char[0] = buffer[j];
//                          //�������һ��
//                        if(x >= max_x-1)
//                        {
//                           	x = 0;
//                           	y ++;
//                        }                                
//                        j++;
//                        CHINA_FLAG = 1;
//       			    }
//                    else if(CHINA_FLAG)
//					{      //���꺺�ֵ��Ұ�߲���ʾ
//                      	tmp_char[1] = buffer[j];
//						//����ģ 
//						// ��ʾ���� 
//						TFT_ShowWord_SD(x*8,y*16,tmp_char,BLACK,WHITE); 
//                        j++;
//                        x += 2;
//                        CHINA_FLAG = 0;
//					}
//				}   
//			    //��ʾ����19�о�ͣס�Ȱ�����ʾ��һ��
//				if(y >= max_y-1)
//                {
//                   	while(1)
//					{
//	                    if(Convert_Pos())	//�õ�����ֵ
//						{	
//							if(tp_pixlcd.x>192&&tp_pixlcd.y>298) //��һҳ
//							{
//								TFT_ClearArea(0,16,240,320-32,WHITE); //�����ʾ��
//			                	y = 1;  
//								break;
//							}                                                  
//						}
//					}
//				}			                            
//			}
//			j=0;
//			new_Sec++;
//		}
//		Temp_ClusL=FAT_seek_nextCluster(Temp_ClusL);//��FAT���в�����һ���غ�	
//	}
//
//}









