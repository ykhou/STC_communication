 #include <stdio.h>
 #include "paint.h"
 #include "tft.h"
 #include "txt.h"
 #include "touch.h"
 /**************************************************************************************************/
void PaintFrame(void)
{
	//��ʾ������
	TFT_ClearONELINE(0,RED);
    TFT_ShowString(0,0,"CPU:STC15W4K48S4",WHITE,RED);			 
	TFT_ShowString_W25(20,0,2,"��˼��Ƽ�",WHITE,RED);
    //TFT_ShowString_SD(22,0,"����ѧԺ",WHITE,RED);
	TFT_ClearArea(0,16,200,320-32,WHITE); //�����ʾ��
	TFT_ClearArea(200,16,40,320-32,LIGHTBLUE); //���������
	TFT_ClearONELINE(19,BLUE);
  TFT_ShowString_W25(0,19,2,"������",WHITE,RED);
	TFT_ShowString(9,19,"LCDX:",RED,BLUE);
	TFT_ShowString(20,19,"LCDY:",RED,BLUE);

	TFT_ShowString(25,1,"clear",BLACK,LGRAY);
	TFT_ShowString(25,2,"     ",MAGENTA,MAGENTA);
	TFT_ShowString(25,3,"--1--",BLACK,LGRAY); LCD_DrawPoint_big(220,64+8,1,BLUE);
	TFT_ShowString(25,5,"--3--",BLACK,LGRAY); LCD_DrawPoint_big(218,96+6,3,BLUE);
	TFT_ShowString(25,7,"--5--",BLACK,LGRAY);LCD_DrawPoint_big(217,128+5,5,BLUE);
	TFT_ShowString(25,9,"--7--",BLACK,LGRAY);LCD_DrawPoint_big(216,160+4,7,BLUE);
	TFT_ShowString(25,11,"--9--",BLACK,LGRAY);LCD_DrawPoint_big(215,192+3,9,BLUE);
	TFT_ShowString(25,13,"-----",BLACK,LGRAY);
	LCD_DrawPoint_big(202,224+8,15,RED);LCD_DrawPoint_big(222,224+8,15,YELLOW);	// 232
	LCD_DrawPoint_big(202,247+8,15,BLUE);LCD_DrawPoint_big(222,247+8,15,CYAN);
	LCD_DrawPoint_big(202,270+8,15,BLACK);LCD_DrawPoint_big(222,270+8,15,BROWN);
}
//��ʾ��������
void ZuoBiao(void)
{
	unsigned char showzf[4];
	sprintf(showzf,"%d",tp_pixlcd.x);
	TFT_ShowString(9+5,19,"    ",RED,BLUE);TFT_ShowString(9+5,19,showzf,RED,BLUE);
	sprintf(showzf,"%d",tp_pixlcd.y);
	TFT_ShowString(20+5,19,"    ",RED,BLUE);TFT_ShowString(20+5,19,showzf,RED,BLUE);
}

//��ͼ����
//n:���ʴ�ϸ
void PaintTouch(void) 
{
	unsigned int color=BLUE;	
	unsigned char n=1;
	PaintFrame();
    while(1)
	{
		laidian();  	
		if(!Touch_IRQ())
		{
			if(Convert_Pos())	//�õ�����ֵ
			{
				if(tp_pixlcd.x<200 && tp_pixlcd.y>16 && tp_pixlcd.y<320-22 )//��ͼ��
				{
					LCD_DrawPoint_big(tp_pixlcd.x,tp_pixlcd.y,n,color); 
				} 

				else if(tp_pixlcd.x<48&&tp_pixlcd.y>298)	  //�±�ǩ��"������"
					break; 				
				else if(tp_pixlcd.x>200&&tp_pixlcd.y>32&&tp_pixlcd.y<320-16)	 //������
				{
			   		if(  tp_pixlcd.y<32+16)//������ͼ
					{	TFT_ClearArea(0,16,200,320-32,WHITE); 	}
					else if(tp_pixlcd.y>64&&tp_pixlcd.y<80)	//���ʴ�1
					{   n=1;			}						
					else if(tp_pixlcd.y>96 && tp_pixlcd.y<96+16)//���ʴ�3
					{   n=3;			}
					else if(tp_pixlcd.y>128 &&tp_pixlcd.y<128+16)//���ʴ�5
					{   n=5;			}						
					else if(tp_pixlcd.y>160 &&tp_pixlcd.y<160+16)//���ʴ�7
					{   n=7;			}
					else if(tp_pixlcd.y>192 &&tp_pixlcd.y<192+16)//���ʴ�9
					{   n=9;			}
					//��ɫѡ��
					else if(tp_pixlcd.y>224+4 &&tp_pixlcd.y<224+15+4)
					{ 	
						if(tp_pixlcd.x<220)  color=RED;
						else color=YELLOW;
					}
					else if(tp_pixlcd.y>247+4 &&tp_pixlcd.y<247+15+4)
					{ 	
						if(tp_pixlcd.x<220)  color=BLUE;
						else color=CYAN;
					}
					else if(tp_pixlcd.y>270+4 &&tp_pixlcd.y<270+15+4)
					{ 	
						if(tp_pixlcd.x<220)  color=BLACK;
						else color=BROWN;
					}
				}

				ZuoBiao();	
			}
		}	
	}
}	    	
