
#include "tft.h"
#include "bin.h"
#include "txt.h"
#include "touch.h"
#include "delay.h"
#include "huanjing.h"
#include "iic.h"
#include "interrupt.h"
#include "STC15F2K60S2.h"

#define	  SlaveAddress   0x46 //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A

sbit CG_WDO = P4^4;	 //������	
/**
  dongtai.c����
 ���ն� ��������IICЭ�飩
 ��ʪ�� �� P44
*/


unsigned char xdata BH1750_BUF[3];	
unsigned char humitureT[3],humitureH[3]; //�¶�,ʪ��
unsigned char xdata DispBuf[6]; 	//���ն�
unsigned char T_H,T_L,R_H,R_L; //�¶�,ʪ�� HΪ����λ��LΪС��λ

void HuanjingIOInit(void)
{  
P4M0 |=(1<<4);
P4M1 &=~(1<<4);	 
Init_IIC();
}
/****************************************����Ϊ���նȹ��ܳ���***********************************************/
void Single_Write_BH1750(unsigned char REG_Address)
{
    I2CStart();                  //��ʼ�ź�
    I2CSendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2CSendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    I2CStop();                   //����ֹͣ�ź�
}
//*********************************************************
//��������BH1750�ڲ�����
//*********************************************************
void Multiple_read_BH1750(void)
{   unsigned char i;	
    I2CStart();                          //��ʼ�ź�
    I2CSendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	
	 for (i=0; i<3; i++)                      //������ȡ6����ַ���ݣ��洢��BH1750_BUF
    {
        BH1750_BUF[i] = I2CRecvByte();          //BH1750_BUF[0]�洢0x32��ַ�е�����
        if (i == 3)
        {

           I2CSendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {		
          I2CSendACK(0);                //��ӦACK
       }
   }

    I2CStop();                          //ֹͣ�ź�
    delayms(5);
}


//��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
void Init_BH1750(void)
{
   Single_Write_BH1750(0x01);  
}

//*********************************************************
//������********
//*********************************************************
void BH1750(void)
{  

   unsigned int     GZData,TempData;    //����  
						

    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode

    delayms(180);              //��ʱ180ms

    Multiple_read_BH1750();       //�����������ݣ��洢��BH1750_BUF��

    GZData=BH1750_BUF[0];
    GZData=(GZData<<8)+BH1750_BUF[1];//�ϳ����� 
    
    GZData=(unsigned int)(((float)GZData)/1.2);
 
	DispBuf[0]=GZData/10000+0x30 ;
    TempData=GZData%10000;   //ȡ������
	DispBuf[1]=TempData/1000+0x30 ;
    TempData=TempData%1000;    //ȡ������
    DispBuf[2]=TempData/100+0x30   ;
    TempData=TempData%100;     //ȡ������
    DispBuf[3]=TempData/10+0x30    ;
    TempData=TempData%10;      //ȡ������
    DispBuf[4]=TempData+0x30; 	
	DispBuf[5]='\0';

	TFT_ShowString(7,3,DispBuf,BLACK,CYAN);
} 


/***************************************����Ϊ��ʪ�ȹ��ܳ���************************************************/

void DHT_IO_OUT(void)
{
	CG_WDO=1;
	P4M0 |=(1<<4);
	P4M1 &=~(1<<4);	
}
void DHT_IO_IN(void)
{
	CG_WDO=1;
	P4M0 &=~(1<<4);
	P4M1 |=(1<<4);	
}

void DH11Start(void)//��ʼ�ź�
{
    DHT_IO_OUT();
    CG_WDO=0;
    delayms(30); //��������18ms
    DHT_IO_IN(); //�л�������
    delay(30);//20-40us
}

unsigned char DH11ReceiveOneByte(void)//����һ���ֽ�,��λ��ǰ
{
    unsigned char i,temp=0;
    for(i=0;i<8;i++)
    {
        temp <<=1;
        while(CG_WDO);//�ȴ�����50us
        delay(40);
        while(!CG_WDO);//�ȴ�����26-28us
        delay(35);
        if(CG_WDO==1)  //��ʾ�ߵ�ƽ����ʱ�䳬��28Ϊ����1 ����Ϊ��
        { 
            temp |=(1<<0);  
        }   
     }
     return temp;
}

void DH11GetHumiture(void)//�������� 5��:�¶�����,С��,ʪ������,С��,У���
{						   //������С��λΪ0,���Բ���Ҫ
    unsigned char check,num_check;

    DH11Start();//��ʼ�ź�
    while(CG_WDO);//�ȴ���Ӧ�ź�����
    delay(80);
    while(!CG_WDO);//�ȴ���Ӧ�ź�����
    delay(80);
 
    R_H=DH11ReceiveOneByte();
    R_L=DH11ReceiveOneByte();  //������С��λΪ0  
    T_H=DH11ReceiveOneByte();
    T_L=DH11ReceiveOneByte();  //������С��λΪ0
    num_check=DH11ReceiveOneByte();

    while(CG_WDO);//�ȴ���Ӧ�ź�����
    delay(50);

    DHT_IO_OUT(); //�ͷ�����

    check=R_H+R_L+T_H+T_L;
    if(check==num_check)
    {
        humitureT[0]=T_H/10+0x30;
		humitureT[1]=T_H%10+0x30;
		humitureT[2]='\0';	 
        humitureH[0]=R_H/10+0x30;
		humitureH[1]=R_H%10+0x30;
		humitureH[2]='\0';
    }
}


/****************************************����Ϊ��������ʾ����***********************************************/


void HuanjingMFrame(void)
{
	TFT_ClearArea(0,16,240,320-16,LIGHTBLUE);
	TFT_ClearONELINE(19,BLUE);
    TFT_ShowString_W25(0,19,2,"������",WHITE,RED);

	TFT_ShowString_W25(8,1,2,"��ǰ�������:",BROWN,WHITE);
	TFT_ShowString_W25(0,3,2,"���ն�:",WHITE,BLUE);
	TFT_ShowString_W25(13,3,2,"�¶�:",WHITE,BLUE);
	TFT_ShowString_W25(21,3,2,"ʪ��:",WHITE,BLUE);
}
void HumitureFrame(void)
{
   TFT_ClearArea(20,70,200,200,LIGHTBLUE);
   //*********����
    LCD_DrawLine(20,70,20,270,BLUE); LCD_DrawLine(20,70,15,80,BLUE);LCD_DrawLine(20,70,25,80,BLUE);
    LCD_DrawLine(20,270,220,270,BLUE); LCD_DrawLine(220,270,210,265,BLUE);LCD_DrawLine(220,270,210,275,BLUE);
   //*********�̶�
    LCD_DrawLine(20,90,25,90,BLUE);    //���20,ÿһ����С�̶�ֵ��Ӧ2������ ����=270-�¶�ֵ*2
    LCD_DrawLine(20,110,25,110,BLUE);
    LCD_DrawLine(20,130,25,130,BLUE);
    LCD_DrawLine(20,150,25,150,BLUE);
    LCD_DrawLine(20,170,25,170,BLUE);
    LCD_DrawLine(20,190,25,190,BLUE);
    LCD_DrawLine(20,210,25,210,BLUE);
    LCD_DrawLine(20,230,25,230,BLUE);
    LCD_DrawLine(20,250,25,250,BLUE);  
}

void HuanjJiancTouch(void)
{
    unsigned int OldX1=20,OldY1=270,NewX1,NewY1; //�¶�
    unsigned int OldX2=20,OldY2=270,NewX2,NewY2; //ʪ��
    unsigned char cidu=8;//���ߵĳ߶ȱ���
    unsigned char THTemp[3]={0};

	HuanjingMFrame();
	HuanjingIOInit();
    HumitureFrame();
	
	Init_BH1750();
	Time1_Init20MS();//��ʱ1s���ڸ��´�����ʱ��

    TR1=1;//��ʼ����


	while(1)
	 {
		if(!Touch_IRQ()) //�д�������
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //�±�ǩ��"������"
				{TR1=0;ET1 =0;CountNumber=0;return;} //�رն�ʱ��

			 }
		 }
	//���նȴ�����
		 BH1750();
	//�����¶�����

        if(CountNumber>80)//��ʱʱ�����1s,ȡһ��ֵ    20ms* CountNumber
        {
            CountNumber=0;
			DH11GetHumiture();
			TFT_ShowString(18,3,humitureT,YELLOW,LIGHTBLUE);  //�¶�����
			TFT_ShowString(26,3,humitureH,GREEN,LIGHTBLUE);  //ʪ������

		    if(OldX1<100*2)
            {
                NewX1=OldX1+cidu;NewY1=270-T_H*2;
                LCD_DrawLine(OldX1,OldY1,NewX1,NewY1,YELLOW);  
                OldX1=NewX1;OldY1=NewY1;
        
                NewX2=OldX2+cidu;NewY2=270-R_H*2;
                LCD_DrawLine(OldX2,OldY2,NewX2,NewY2,GREEN);  
                OldX2=NewX2;OldY2=NewY2;
            }
            else  
            {
              OldX1=20;
              OldX2=20;
              HumitureFrame();
            }
        }
	  }
}
