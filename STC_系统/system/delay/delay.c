#include "intrins.h"
void delay(unsigned int count)  // /* X1ms */
{
		while(count--)
		{_nop_();_nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();
		 _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		 }
}
void delayms(unsigned int count)  // /* X1ms */2200=24M
{
        int i,j;
        for(i=0;i<count;i++)
                for(j=0;j<2680;j++);

}
