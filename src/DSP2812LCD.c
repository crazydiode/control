//BUS  LCD   
#include "DSP28_Device.h"


extern unsigned int * LED_COMM;
extern unsigned int * LED_DATA;


///GpioDataRegs.GPBDAT.bit.GPIOB10=NAND RNB
///////////////////////////////////////////////
extern unsigned int XPOS,YPOS;



void delay1(unsigned int);
void ldelay1(unsigned int); 
/////////////////////////////////////////////////
void lcdwc(unsigned int c)
{  
   ldelay1(100);
   *LED_COMM=c;
}
/////////////////////////////////////
void lcdwd(unsigned int d)
{  
   ldelay1(100);
   *LED_DATA=d;
  // ldelay(1000);
  // d=LED_RDATA;
}
///////////////////////////////////////////////
void lcdpos(void)
{  
   lcdwc(0x00);     //¹â±ê·µ»Ø
   XPOS&=0x000f;
   YPOS&=0x0003;
   if(YPOS==0x0000)
      lcdwc(XPOS|0x80);
   else if(YPOS==0x0001)
      lcdwc((XPOS+0x40)|0x80);
}
///////////////////////////////////////////////////////
void lcdfill(unsigned int n)
{
   for(YPOS=0;YPOS<2;YPOS++)
      for(XPOS=0;XPOS<16;XPOS++)
      {  lcdpos();
         lcdwd(n);  }
}
///////////////////////////////////////////////////////
void lcdreset()
{  ldelay1(1500); lcdwc(0x38);
   ldelay1(1500); lcdwc(0x38);
   ldelay1(1500); lcdwc(0x38);
   lcdwc(0x38);
   lcdwc(0x08);
   lcdwc(0x01);
   lcdwc(0x06);
   lcdwc(0x0c);
}
////////////////////////////////////////////////////////////////////
/*
void lcdwda(unsigned int x,unsigned int y, char *s)
{  YPOS=y;
   for(XPOS=x;XPOS<16;XPOS++)
   {  lcdpos();
      lcdwd(*s);
      s++;
      delay1(4000);
   }
}
*/
void lcdw_chars(unsigned int x,unsigned int y,unsigned char length,char *s)
{
	
	YPOS=y;
	XPOS=x;
	lcdpos();
	for(XPOS;XPOS<length+x;XPOS++){
		lcdwd(*s);
		s++;
		delay1(4000);
	}
}

////////////////////////////////////////////////////

void Lcd_Init()
{
	lcdreset();
	lcdw_chars(0,0,9,"Speedset:");  
	lcdw_chars(0,1,10,"Speedreal:");  
	//lcdw_chars(8,0,2,"YL");
}


	
void ldelay1(unsigned int k)
{
	while(k>0)
	{
		k--;
		delay1(500);
	}
}	

void delay1(unsigned int t)
{
	while(t>0)
		t--;
}


