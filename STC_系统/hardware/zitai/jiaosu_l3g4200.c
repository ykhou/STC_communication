#include "STC15F2K60S2.h"
#include "tft.h"
#include "txt.h"
#include "iic.h"
#include "delay.h"
#include "jiaosu_l3g4200.h"

#include  <math.h>    //Keil library  



#define	L3G4200_AD   0xD2	  //三轴角速度陀螺仪L3G4200    


//**********L3G4200D???????*********
#define WHO_AM_I 0x0F
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0x25
#define OUT_TEMP 0x26
#define STATUS_REG 0x27
#define OUT_X_L 0x28
#define OUT_X_H 0x29
#define OUT_Y_L 0x2A
#define OUT_Y_H 0x2B
#define OUT_Z_L 0x2C
#define OUT_Z_H 0x2D
#define FIFO_CTRL_REG 0x2E
#define FIFO_SRC_REG 0x2F
#define INT1_CFG 0x30
#define INT1_SRC 0x31
#define INT1_TSH_XH 0x32
#define INT1_TSH_XL 0x33
#define INT1_TSH_YH 0x34
#define INT1_TSH_YL 0x35
#define INT1_TSH_ZH 0x36
#define INT1_TSH_ZL 0x37
#define INT1_DURATION 0x38


//***************************************************
unsigned char L3G4200BUF[7]='\0';                         //???????   
unsigned char L3G4200DISPLAY[5]='\0';   
//******?????*******************************************

void Single_Write_L3G4200(unsigned char REG_Address,unsigned char REG_data)
{
    I2CStart();                  //????
    I2CSendByte(L3G4200_AD);   //??????+???
    I2CSendByte(REG_Address);    //???????,?????pdf22? 
    I2CSendByte(REG_data);       //???????,?????pdf22? 
    I2CStop();                   //??????
}

//********?????*****************************************
unsigned char Single_Read_L3G4200(unsigned char REG_Address)
{  unsigned char REG_data;
    I2CStart();                          //????
    I2CSendByte(L3G4200_AD);           //??????+???
    I2CSendByte(REG_Address);                   //????????,?0??	
    I2CStart();                          //????
    I2CSendByte(L3G4200_AD+1);         //??????+???
    REG_data=I2CRecvByte();              //???????
	  I2CSendACK(1);   
	  I2CStop();                           //????
    return REG_data; 
}
//*********************************************************
//
//????L3G4200???????,????0x28~0x2D
//
//*********************************************************
void Multiple_Read_L3G4200(void)
{   unsigned char i;
    I2CStart();                          //????
    I2CSendByte(L3G4200_AD);           //??????+???
    I2CSendByte(0x28);                   //????????,?0x32??	
    I2CStart();                          //????
    I2CSendByte(L3G4200_AD+1);         //??????+???
	 for (i=0; i<6; i++)                      //????6?????,???BUF
    {
        L3G4200BUF[i] = I2CRecvByte();          //BUF[0]??0x32??????
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

//???L3G4200,???????pdf????************************
void Init_L3G4200()
{
   Single_Write_L3G4200(CTRL_REG1, 0x0f);   //
   Single_Write_L3G4200(CTRL_REG2, 0x00);   //
   Single_Write_L3G4200(CTRL_REG3, 0x08);   //
   Single_Write_L3G4200(CTRL_REG4, 0x30);  //+-2000dps
   Single_Write_L3G4200(CTRL_REG5, 0x00);
}

//??x?
void DISP_L3G4200_x()
{   
	int temp_data;
	  float temp ;       
    L3G4200BUF[0]= Single_Read_L3G4200(OUT_X_L);
    L3G4200BUF[1]= Single_Read_L3G4200(OUT_X_H); //??X???
    temp_data=(L3G4200BUF[1]<<8)+L3G4200BUF[0];  //????   
	  temp=(float)temp_data*0.07; // ???? ?9?,2000?/???
		temp_data=(int)temp;
		
	if(temp_data<0){
	temp_data=-temp_data;
		L3G4200DISPLAY[0]='-';   
	}
	else L3G4200DISPLAY[0]='+'; 
 
    L3G4200DISPLAY[1]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    L3G4200DISPLAY[2]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    L3G4200DISPLAY[3]=temp_data+0x30; 
	  L3G4200DISPLAY[4]='\0';
	  
		TFT_ShowString(0,15,"X:",BLACK,CYAN);
	  TFT_ShowString(2,15,L3G4200DISPLAY,BLACK,CYAN);
}
void DISP_L3G4200_y()
{   
	int temp_data;
	  float temp  ;                   //??
    L3G4200BUF[2]= Single_Read_L3G4200(OUT_Y_L);
    L3G4200BUF[3]= Single_Read_L3G4200(OUT_Y_H); //??X???
    temp_data=(L3G4200BUF[3]<<8)+L3G4200BUF[2];  //????   
	  temp=(float)temp_data*0.07; // ???? ?9?,2000?/???
		temp_data=(int)temp;
		
	if(temp_data<0){
	temp_data=-temp_data;
		L3G4200DISPLAY[0]='-';   
	}
	else L3G4200DISPLAY[0]='+'; 
 
    L3G4200DISPLAY[1]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    L3G4200DISPLAY[2]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    L3G4200DISPLAY[3]=temp_data+0x30; 
	  L3G4200DISPLAY[4]='\0';
	  
		TFT_ShowString(0,16,"Y:",BLACK,CYAN);
	  TFT_ShowString(2,16,L3G4200DISPLAY,BLACK,CYAN);
}
void DISP_L3G4200_z()
{   
	int temp_data;
	  float temp ;                    //??
    L3G4200BUF[4]= Single_Read_L3G4200(OUT_Z_L);
    L3G4200BUF[5]= Single_Read_L3G4200(OUT_Z_H);//??Z???
	
    temp_data=(L3G4200BUF[5]<<8)+L3G4200BUF[4];  //????   
	  temp=(float)temp_data*0.07; // ???? ?9?,2000?/???
		temp_data=(int)temp;
		
	if(temp_data<0){
	temp_data=-temp_data;
		L3G4200DISPLAY[0]='-';   
	}
	else L3G4200DISPLAY[0]='+'; 
 
    L3G4200DISPLAY[1]=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //????
    L3G4200DISPLAY[2]=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //????
    L3G4200DISPLAY[3]=temp_data+0x30; 
	  L3G4200DISPLAY[4]='\0';
	  
		TFT_ShowString(0,17,"Z:",BLACK,CYAN);
	  TFT_ShowString(2,17,L3G4200DISPLAY,BLACK,CYAN);
}
void DISP_L3G4200_temp()
{   
	unsigned char Temperature;
Temperature=Single_Read_L3G4200(OUT_TEMP); //????	
	  
    L3G4200DISPLAY[0]=Temperature/100+0x30   ;
    Temperature=Temperature%100;     //????
    L3G4200DISPLAY[1]=Temperature/10+0x30    ;
    Temperature=Temperature%10;      //????
    L3G4200DISPLAY[2]=Temperature+0x30; 
	  L3G4200DISPLAY[3]='\0';
	  
	  TFT_ShowString_W25(0,18,2,"温度:",BLACK,CYAN);
	  TFT_ShowString(5,18,L3G4200DISPLAY,BLACK,CYAN);



}



void L3G4200Convert()
{
//	unsigned char devid;
 //devid=Single_Read_L3G4200(0X00);//??????0XE5,????
	TFT_ShowString_W25(0,14,2,"角速度:",WHITE,BLUE);
	
	   DISP_L3G4200_x();                   //---------??X?
    DISP_L3G4200_y();                   //---------??Y?
    DISP_L3G4200_z();                   //---------??Z?
	//  DISP_L3G4200_temp();
}

