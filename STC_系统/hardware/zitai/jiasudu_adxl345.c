#include "STC15F2K60S2.h"
#include "tft.h"
#include "txt.h"
#include "iic.h"
#include "delay.h"
#include "jiasudu_adxl345.h"
#include  <math.h>    //Keil library  



#define	I2CAD   0xA6	  //三轴加速度ADXL345,ALT  ADDRESS引脚接地时地址为0xA6,接电源时为0x3A
//***************************************************
unsigned char ADXL345BUF[7]='\0';                         //???????   
unsigned char ADXL345DISPLAY[8]='\0';   
//******?????*******************************************

void Single_Write_ADXL345(unsigned char REG_Address,unsigned char REG_data)
{
    I2CStart();                  //????
    I2CSendByte(I2CAD);   //??????+???
    I2CSendByte(REG_Address);    //???????,?????pdf22? 
    I2CSendByte(REG_data);       //???????,?????pdf22? 
    I2CStop();                   //??????
}

//********?????*****************************************
unsigned char Single_Read_ADXL345(unsigned char REG_Address)
{  unsigned char REG_data;
    I2CStart();                          //????
    I2CSendByte(I2CAD);           //??????+???
    I2CSendByte(REG_Address);                   //????????,?0??	
    I2CStart();                          //????
    I2CSendByte(I2CAD+1);         //??????+???
    REG_data=I2CRecvByte();              //???????
	I2CSendACK(1);   
	I2CStop();                           //????
    return REG_data; 
}
//*********************************************************
//
//????ADXL345???????,????0x32~0x37
//
//*********************************************************
void Multiple_Read_ADXL345(void)
{   unsigned char i;
    I2CStart();                          //????
    I2CSendByte(I2CAD);           //??????+???
    I2CSendByte(0x32);                   //????????,?0x32??	
    I2CStart();                          //????
    I2CSendByte(I2CAD+1);         //??????+???
	 for (i=0; i<6; i++)                      //????6?????,???BUF
    {
        ADXL345BUF[i] = I2CRecvByte();          //BUF[0]??0x32??????
        if (i == 5)
        {
           I2CSendACK(1);                //?????????NOACK
        }
        else
        {
          I2CSendACK(0);                //??ACK
       }
   }
    I2CStop();                          //????
    delayms(5);
}


//*****************************************************************

//???ADXL345,???????pdf????************************
void Init_ADXL345()
{
   Single_Write_ADXL345(0x31,0x0B);   //????,??16g,13???
   Single_Write_ADXL345(0x2C,0x08);   //?????12.5 ??pdf13?
   Single_Write_ADXL345(0x2D,0x08);   //??????   ??pdf24?
   Single_Write_ADXL345(0x2E,0x80);   //?? DATA_READY ??
   Single_Write_ADXL345(0x1E,0x00);   //X ??? ????????????pdf29?
   Single_Write_ADXL345(0x1F,0x00);   //Y ??? ????????????pdf29?
   Single_Write_ADXL345(0x20,0x05);   //Z ??? ????????????pdf29?
}

//??x?
void DISP_ADXL345_x()
{   
	int temp_data;
	int  dis_data;                       //??
    dis_data=(ADXL345BUF[1]<<8)+ADXL345BUF[0];  //????   
	if(dis_data<0){
	dis_data=-dis_data;
		ADXL345DISPLAY[0]='-';   
	}
	else ADXL345DISPLAY[0]='+';

    temp_data=(int)((float)dis_data*3.9);  //???????,??ADXL345?????4?
    
	ADXL345DISPLAY[1]=temp_data/10000+0x30 ;
temp_data=temp_data%10000;   //????
	ADXL345DISPLAY[2]=temp_data/1000+0x30 ;
	ADXL345DISPLAY[3]='.';
    temp_data=temp_data%1000;    //????
    ADXL345DISPLAY[4]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    ADXL345DISPLAY[5]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    ADXL345DISPLAY[6]=temp_data+0x30; 
	ADXL345DISPLAY[7]='\0';
	TFT_ShowString(0,10,"X:",BLACK,CYAN);
	TFT_ShowString(2,10,ADXL345DISPLAY,BLACK,CYAN);
}

//***********************************************************************
//??y?
void DISP_ADXL345_y()
{   	int temp_data;
	int  dis_data;                       //??
    dis_data=(ADXL345BUF[3]<<8)+ADXL345BUF[2];  //????   
	if(dis_data<0){
	dis_data=-dis_data;
		ADXL345DISPLAY[0]='-';   
	}
	else ADXL345DISPLAY[0]='+';

 temp_data=(int)((float)dis_data*3.9); 
    
	ADXL345DISPLAY[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	ADXL345DISPLAY[2]=temp_data/1000+0x30 ;
	ADXL345DISPLAY[3]='.';
    temp_data=temp_data%1000;    //????
    ADXL345DISPLAY[4]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    ADXL345DISPLAY[5]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    ADXL345DISPLAY[6]=temp_data+0x30; 	
	ADXL345DISPLAY[7]='\0';
	TFT_ShowString(0,11,"Y:",BLACK,CYAN);
	TFT_ShowString(2,11,ADXL345DISPLAY,BLACK,CYAN);
}

//***********************************************************************
//??z?
void DISP_ADXL345_z()
{    	int temp_data;
	int  dis_data;                       //??
    dis_data=(ADXL345BUF[5]<<8)+ADXL345BUF[4];  //????   
	if(dis_data<0){
	dis_data=-dis_data;
		ADXL345DISPLAY[0]='-';   
	}
	else ADXL345DISPLAY[0]='+';

 temp_data=(int)((float)dis_data*3.9); 
    
	ADXL345DISPLAY[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	ADXL345DISPLAY[2]=temp_data/1000+0x30 ;
	ADXL345DISPLAY[3]='.';
    temp_data=temp_data%1000;    //????
    ADXL345DISPLAY[4]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    ADXL345DISPLAY[5]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    ADXL345DISPLAY[6]=temp_data+0x30; 	
	ADXL345DISPLAY[7]='\0';
	TFT_ShowString(0,12,"Z:",BLACK,CYAN);
	TFT_ShowString(2,12,ADXL345DISPLAY,BLACK,CYAN);
}
void ADXL345Convert()
{
//	unsigned char devid;
 //devid=Single_Read_ADXL345(0X00);//??????0XE5,????
	TFT_ShowString_W25(0,9,2,"加速度:",WHITE,BLUE);
	Multiple_Read_ADXL345();  
	
    DISP_ADXL345_x();                   //---------??X?
    DISP_ADXL345_y();                   //---------??Y?
    DISP_ADXL345_z();                   //---------??Z?
}

