#include "string.h"
#include "STC15F2K60S2.h"
#include "txt.h"
#include "tft.h"
#include "font.h"
#include "delay.h"
#include "flash.h"
#include "fontcheck.h"

extern unsigned char Y_LEN;//全局变量用于消息一直往下打印

void TFT_ShowWord_W25(unsigned int x,unsigned int y,unsigned int size_x,unsigned int size_y,unsigned char *Disp_char,unsigned int PColor,unsigned int BColor)
{
    unsigned int i,j,xCount=0;
	unsigned int pxSize;
	TFT_AddressSet(x,y,x+size_x-1,y+size_y-1); //设置区域 
	pxSize =(size_x/8+(size_x%8?1:0))*size_y;   //单个字符的数组个数

	for(j=0;j<pxSize;j++)	  //size_x*size_y/8为字节个数
	{
		for(i=0;i<8;i++)	    //高位在前，低位在后，取字模时匹配就OK
		{ 						
			if(*Disp_char&(0x80>>i))  //最高位为左移7位
			{
				//对应于取字模时的阴码
				TFT_WR_DATA(PColor);
			} 
			else
			{				
				TFT_WR_DATA(BColor);
			}
			++xCount;	
			if(xCount%size_x==0){xCount=0;break;} //对于字符宽不是8倍数的字符
		}
	  	Disp_char++;
	 }		
}
/**
**size参数可选1、2、3、4，其余数值无效
**/
void TFT_ShowString_W25(unsigned int x,unsigned int y,unsigned int size,unsigned char *ptr,unsigned int PenColor,unsigned int BackColor)
{         
 	unsigned char i,val=0; //一定要初始化，不然xlen在第二次调用时会有问题

	unsigned char temp[2]={0};
	unsigned int xlen;
	unsigned int ylen;	
  unsigned int max_ylen;
	unsigned int max_xlen;
	unsigned long pxSize=0,px;	  //数组个数
	unsigned long position=0;	
	unsigned char p[129]={0};	//32*32汉字4*32=128个字节	 
	unsigned char p0[65]={0};

	if(size==1) px=12;
	else if(size==2) px=16;
	else if(size==3) px=24;
	//else if(size==4) px=28
	else if(size==4) px=32;
	else return;  										  	

  max_xlen=240*2/px;
	max_ylen=320/px;      	//最多显示240/8=30列 320/16=20行 
																								  
	for(i=0;i<strlen(ptr);i++)
	{
	 	temp[0]=ptr[i];
		temp[1]=ptr[i+1];
		
		xlen =(x+i+val)%max_xlen;	   	 		
		ylen =((x+i+val)/max_xlen+y)%max_ylen;  


		if(temp[0]<128)		//为ASCII码
		{
			pxSize =(px/16+(px%16?1:0))*px;  //单个字符的数组个数		   ?????
			position=(unsigned long)temp[0]*(unsigned long)pxSize;
			if(px==12)	 { Spi_PageRead(p, ftinfo.asc12_addr+position, pxSize); } 
			else if(px==16){ Spi_PageRead(p, ftinfo.asc16_addr+position, pxSize); }
			else if(px==24){ Spi_PageRead(p, ftinfo.asc24_addr+position, pxSize); }
			else if(px==32){ Spi_PageRead(p, ftinfo.asc32_addr+position, pxSize); } 	
			TFT_ShowWord_W25(xlen*px/2,ylen*px,px/2,px,p,PenColor,BackColor); 	
		}
		else 			   //为汉字
	   	{	
			if(xlen==max_xlen-1)   //最后一列不能放下一个汉字
			{
				TFT_ShowWord_W25(xlen*px/2,ylen*px,px/2,px,p0,PenColor,BackColor);  //显示“ ”空白填补
				val++;	
				i--;//指针还是指向原汉字,所以要减一			
			}
			else
			{	//第一个汉字GBK编码范围为0x81~0xFE
		    	//第二个汉字GBK编码范围为0x40~0x7E 0x80~0xFE
				pxSize =(px/8+(px%8?1:0))*px;   //单个字符的数组个数			??????????????		
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
	Y_LEN = ylen+1; //记录下用到的行数+下一行
	Y_LEN %=max_ylen;
}


///*********************************************************************
//名称TXT的核心程序
//描述:文件的开始簇号，大小，文件名，这些是重要数据
//功能:
//返回值:
//输入值:TXT文件数据
//*********************************************************************/
//void DISP_TXT(unsigned char cjd)
//{
//	unsigned long Temp_ClusL,new_Sec;
//	
//	unsigned int i,j=0;
//	unsigned long total=0;
//	unsigned char tmp_char[2],CHINA_FLAG=0;
//	unsigned char x=0,y=0;
//	unsigned char max_x=30,max_y=20;//最多显示240/8=30列 320/16=20行 	
//	TFT_ShowString_SD(0,19,"                        下一页",WHITE,RED);
//	y=1;
//	Temp_ClusL= doc_info.DOC_FstClusL[cjd];//将首簇赋值 
//	while(Temp_ClusL!=0x0fffffff)//一直到最后一簇
//	{
//		new_Sec=Cluster_to_Sector(Temp_ClusL);	    //算簇号的首扇区  =(某簇号-根目录簇号)*每簇扇区数+根目录开始扇区号
//		for(i=0;i<fat_info.BPB_perCluster_Sector_number;i++)//查每簇的所有扇区
//		{
//                   //* 读一扇区内容
//		    SD_read_sector(new_Sec,buffer);
//			while(j<512)
//			{       
//			  	//* 如果显示完文件就退出 
//				total++;
//        		if(total>doc_info.DOC_Filesize[cjd])
//				{
//					TFT_ShowString_SD(0,19,"                          返回",WHITE,RED);
//                   	while(1)
//					{
//	                    if(Convert_Pos())	//得到坐标值
//						{	
//							if(tp_pixlcd.x>208&&tp_pixlcd.y>298)	  //下标签栏"返回"
//								return;
//						}
//					}
//				}
//				 //*回车判断函数  函数里只设置了 可以连续判断3个回车    
//             //如果是ASCII码
//       			if(buffer[j] < 0x80)
//                {                            
//  		               if(buffer[j]==0x0d&&j==511) break;			//判断回车的两个字节是否在两个 扇区的接合之处   
//					   if(buffer[j]==0x0a&&j==0){j++;y++;x=0;}	//这样可能会有丢字节现象		
//		
//					  if((buffer[j]==0x0d)&&(buffer[j+1]==0x0a))    //判断是否有回车 
//					   {
//					     j=j+2;								   //字节+2 跳过此回车数据 
//					     y++;								   // 换行
//					      x=0;								   //从第一个显示位置开始显示
//				    	 }
//					  else									   //显示字节
//					    {                                                     
//                 	
//					TFT_ShowWord(x*8,y*16,8,16,ASCII_816[buffer[j]-0x20],BLACK,WHITE); 
//                    j++;            
//                    x++;  	               	
//               // 如果显满一行                                     
//                    if(x >= max_x)
//                    {
//                    	x = 0;
//                        y ++;
//                    }
//					}
//				}
//               //如果是汉字/
//       			else //	  CHINA_FLAG 用此函数做标志 来判断是否为整个字	
//       			{	//只有为整个字才可以送显示	这样可以避免错行		 
//					if(!CHINA_FLAG)				   
//               		{       
//                		 //汉字的左半边只记录数据，不显示
//                       	tmp_char[0] = buffer[j];
//                          //如果显满一行
//                        if(x >= max_x-1)
//                        {
//                           	x = 0;
//                           	y ++;
//                        }                                
//                        j++;
//                        CHINA_FLAG = 1;
//       			    }
//                    else if(CHINA_FLAG)
//					{      //读完汉字的右半边并显示
//                      	tmp_char[1] = buffer[j];
//						//读字模 
//						// 显示出来 
//						TFT_ShowWord_SD(x*8,y*16,tmp_char,BLACK,WHITE); 
//                        j++;
//                        x += 2;
//                        CHINA_FLAG = 0;
//					}
//				}   
//			    //显示超出19行就停住等按键显示下一屏
//				if(y >= max_y-1)
//                {
//                   	while(1)
//					{
//	                    if(Convert_Pos())	//得到坐标值
//						{	
//							if(tp_pixlcd.x>192&&tp_pixlcd.y>298) //下一页
//							{
//								TFT_ClearArea(0,16,240,320-32,WHITE); //清空显示区
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
//		Temp_ClusL=FAT_seek_nextCluster(Temp_ClusL);//在FAT表中查找下一个簇号	
//	}
//
//}









