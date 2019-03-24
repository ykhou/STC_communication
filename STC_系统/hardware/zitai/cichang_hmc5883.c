#include "STC15F2K60S2.h"
#include "tft.h"
#include "txt.h"
#include "iic.h"
#include "delay.h"
#include "cichang_hmc5883.h"
#include  <math.h>    //Keil library  



#define	HMC5883L_AD   0x3C	          //三轴磁场HMC5883L
//***************************************************
unsigned char HMC5883BUF[7]='\0';                         //???????   
unsigned char HMC5883DISPLAY[8]='\0';   
int  Xdata,Ydata,Zdata;  
void Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data)
{
    I2CStart();                  //????
    I2CSendByte(HMC5883L_AD);   //??????+???
    I2CSendByte(REG_Address);    //???????,?????pdf 
    I2CSendByte(REG_data);       //???????,?????pdf
    I2CStop();                   //??????
}

//********??????????*************************
unsigned char Single_Read_HMC5883(unsigned char REG_Address)
{  unsigned char REG_data;
    I2CStart();                          //????
    I2CSendByte(HMC5883L_AD);           //??????+???
    I2CSendByte(REG_Address);                   //????????,?0??	
    I2CStart();                          //????
    I2CSendByte(HMC5883L_AD+1);         //??????+???
    REG_data=I2CRecvByte();              //???????
	I2CSendACK(1);   
	I2CStop();                           //????
    return REG_data; 
}
//******************************************************
//
//????HMC5883??????,????0x3~0x5
//
//******************************************************
void Multiple_read_HMC5883(void)
{   unsigned char i;
    I2CStart();                          //????
    I2CSendByte(HMC5883L_AD);           //??????+???
    I2CSendByte(0x03);                   //????????,?0x3??	
    I2CStart();                          //????
    I2CSendByte(HMC5883L_AD+1);         //??????+???
	 for (i=0; i<6; i++)                      //????6?????,???BUF
    {
        HMC5883BUF[i] = I2CRecvByte();          //BUF[0]????
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

//???HMC5883,???????pdf????****
void Init_HMC5883()
{
     Single_Write_HMC5883(0x02,0x00);  //
}


//***********************************************************************
//??x?
void DISP_HMC5883_x()
{    	int temp_data;
Xdata=HMC5883BUF[0] << 8 | HMC5883BUF[1]; //Combine MSB and LSB of X Data output register
	temp_data=Xdata;
	if(temp_data<0){
	temp_data=-temp_data;
		HMC5883DISPLAY[0]='-';   
	}
	else HMC5883DISPLAY[0]='+';
    
	HMC5883DISPLAY[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	HMC5883DISPLAY[2]=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //????
  HMC5883DISPLAY[3]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
  HMC5883DISPLAY[4]=temp_data/10+0x30    ;
	HMC5883DISPLAY[5]='.';
    temp_data=temp_data%10;      //????
  HMC5883DISPLAY[6]=temp_data+0x30; 	
	HMC5883DISPLAY[7]='\0';
	TFT_ShowString(0,4,"X:",BLACK,CYAN);
	TFT_ShowString(2,4,HMC5883DISPLAY,BLACK,CYAN);
}
void DISP_HMC5883_y()
{    int temp_data;	
Ydata=HMC5883BUF[4] << 8 | HMC5883BUF[5]; //Combine MSB and LSB of Y Data output register
		temp_data=Ydata;
	if(temp_data<0){
	temp_data=-temp_data;
		HMC5883DISPLAY[0]='-';   
	}
	else HMC5883DISPLAY[0]='+';
    
	HMC5883DISPLAY[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	HMC5883DISPLAY[2]=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //????
  HMC5883DISPLAY[3]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
  HMC5883DISPLAY[4]=temp_data/10+0x30    ;
	HMC5883DISPLAY[5]='.';
    temp_data=temp_data%10;      //????
  HMC5883DISPLAY[6]=temp_data+0x30; 	
	HMC5883DISPLAY[7]='\0';
	TFT_ShowString(0,5,"Y:",BLACK,CYAN);
	TFT_ShowString(2,5,HMC5883DISPLAY,BLACK,CYAN);
}
void DISP_HMC5883_z()
{   int temp_data; 	
Zdata=HMC5883BUF[2] << 8 | HMC5883BUF[3]; //Combine MSB and LSB of Z Data output register
	temp_data=Zdata;
	if(temp_data<0){
	temp_data=-temp_data;
		HMC5883DISPLAY[0]='-';   
	}
	else HMC5883DISPLAY[0]='+';
    
	HMC5883DISPLAY[1]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //????
	HMC5883DISPLAY[2]=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //????
  HMC5883DISPLAY[3]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
  HMC5883DISPLAY[4]=temp_data/10+0x30    ;
	HMC5883DISPLAY[5]='.';
    temp_data=temp_data%10;      //????
  HMC5883DISPLAY[6]=temp_data+0x30; 	
	HMC5883DISPLAY[7]='\0';
	TFT_ShowString(0,6,"Z:",BLACK,CYAN);
	TFT_ShowString(2,6,HMC5883DISPLAY,BLACK,CYAN);
}

void DISP_HMC5883_angle()
{   
   double angle;
	 int temp_data; 
   angle= atan2((double)Ydata,(double)Xdata) * (180 / 3.14159265) +180; // angle in degrees,atan2取值-180~180//
	//保持水平时,与Y轴的夹角,0度表示正北，90表示东，180表示南，270表示西
	//由于加了180度，所以指向与X轴相反,即X轴反方向的指向为指南针北极
	//如果不是水平，要通过三轴加速度来修正
    temp_data= angle*10;
	
		HMC5883DISPLAY[0]=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //
		HMC5883DISPLAY[1]=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //
    HMC5883DISPLAY[2]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //
    HMC5883DISPLAY[3]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //
		HMC5883DISPLAY[4]='.';//小数点
    HMC5883DISPLAY[5]=temp_data+0x30; 	
		HMC5883DISPLAY[6]='\0';
			
		TFT_ShowString(0,7,"Angle:",WHITE,BLUE);
		TFT_ShowString(6,7,HMC5883DISPLAY,WHITE,BLUE);		
}
void HMC5883Convert()
{
   Multiple_read_HMC5883();    
  DISP_HMC5883_x();
	DISP_HMC5883_y();
	DISP_HMC5883_z();
	DISP_HMC5883_angle();	
}	
	
	


