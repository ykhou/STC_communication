#include "STC15F2K60S2.h"
#include "bin.h"
#include "tft.h"
#include "flash.h"

/******************************
显示拼图 图片
可以在指定位置 显示指定大小的bin图片
通过采集到的每块拼图所在的开始簇号 
将其显示出来
x1 y1起始坐标  width height 显示的宽度和高度  

cjd为在拼图文件夹里 采集的图片个数 是按顺序排列的 如果是0 表示图片零 1则表示图片1
   把这个数字直接带入到	doc_info.DOC_FstClusL[cjd]里 就得到图片的开始簇号 
   根据图片的对应簇号 在SD卡里采集图片的相应数据
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
//	Temp_ClusL1= doc_info.PIC_FstClusL[cjd];         //将首簇赋值给临时变量 
//	while(Temp_ClusL1!=0x0fffffff)                   //一直到最后一簇
//	{
//		new_Sec1=Cluster_to_Sector(Temp_ClusL1);	    //算簇号的首扇区  =(某簇号-根目录簇号)*每簇扇区数+根目录开始扇区号
//		for(j1=0;j1<fat_info.BPB_perCluster_Sector_number;j1++)     //查每簇的所有扇区  每簇扇区数 有SD卡本身决定
//		{
//		   SD_read_sector(new_Sec1,buffer);						    //读一扇区所有数据 
//    	  for(i1=0;i1<256;i1++)                          //然后写到液晶屏，可以显示256个像素，每个像素16位即2个字节?也就是每两个字节能显示一个像素点 
//	      {  
//		    total+=2;
//       	    if(total>doc_info.PIC_Filesize[cjd])return;	 //判断是否到最后一簇  如果是 跳出函数
		  
	for(i=0;i<(tReadAddr2-tReadAddr1)/2;i++)
	{	  	
		Spi_PageRead(tmp_Char_Model,(unsigned long)tReadAddr1+i,2);
			TFT_AddressSet(x,y,x,y);                     //这里一个点一个点的显示 先扫横行 在扫纵行	   要符号BIN 生成方式 否则产生乱码
		   TFT_WR_DATA8(tmp_Char_Model[1],tmp_Char_Model[0]);
	
	        x++;								//横向自加 加满一行 横向清零 纵向自加 直到扫完整个屏幕
	        if(x==width+x1)                         //检测是否写到屏的边缘 240X320
	        {	 
	        y++;
	        x=x1;
	        if(y==height+y1)	y=y1; 	          
	        }
		i++;
	}
//		   	j1=0;
//			new_Sec1++;     						//扇区数+1
//		}
//		 Temp_ClusL1=FAT_seek_nextCluster(Temp_ClusL1);//在FAT表中查找下一个簇号	 
//   }
}


void DISP_BIN_PT(unsigned int x1,unsigned int y1,unsigned int width,unsigned int height,unsigned char *tmp_Char_Model)
{
	unsigned long len=width*height*2;//颜色两个字节
	unsigned long i;
	unsigned int x,y;
	x=x1; y=y1;
		  
	for(i=0;i<len/2;i++)
	{	  	
	
			TFT_AddressSet(x,y,x,y);                     //这里一个点一个点的显示 先扫横行 在扫纵行	   要符号BIN 生成方式 否则产生乱码
			TFT_WR_DATA8(tmp_Char_Model[i*2+1],tmp_Char_Model[2*i]);
	//	TFT_WR_DATA8(0xf8,0);

	        x++;								//横向自加 加满一行 横向清零 纵向自加 直到扫完整个屏幕
	        if(x==width+x1)                         //检测是否写到屏的边缘 240X320
	        {	 
						y++;
						x=x1;
						if(y==height+y1)	y=y1; 	          
	        }
	}

}
